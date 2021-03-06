/**********************************************************************************************//**
 * \file	Configuration.cpp.
 *
 * Implements the configuration class
 **************************************************************************************************/

#include "StdAfx.h"
#include "Configuration.h"
#include "TlsRegistry.h"
#include "TlsFile.h"
#include "TlsIniFile.h"
#include "MVModel.h"
#include "acuLink.h"

CConfiguration* CConfiguration::theConfig=0;

/**********************************************************************************************//**
 * A macro that defines ampcorfactorini
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

#define AMPCORFACTORINI       _T("\\FFSDISK\\ampCorFactor.ini")

/**********************************************************************************************//**
 * Initializes a new instance of the CConfiguration class
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

CConfiguration::CConfiguration()
{
	InitializeCriticalSection(&csVentMode);
	m_pModel=NULL;

	for(int i=0;i<16;i++)
	{
		dbBufAmpCorFactor[i] = 0x0000;
	}

	dbAmpCorFactorCONV_K=10000;
	dbAmpCorFactorCONV_J=10000;
	dbAmpCorFactorHFO_K=10000;
	dbAmpCorFactorHFO_J=10000;

	m_iFOTventDelaytime=0;
	m_bSpO2ConfigInProgress=false;

	m_wOldConfigVersion=0;
	m_iConfigVersion=3006;
	m_iParaDataHFFlow=0;
	m_iLanguageID=0;
	//m_wHFamp=0;
	m_bBiasFlow=false;
	m_bSPO2registry=true;
	m_bCO2registry=true;
	m_bRISETIMEregistry=true;
	//m_bPRICO=false;

	m_iPRICO_SPO2lowRange=0;
	m_iPRICO_SPO2highRange=0;
	m_iPRICO_FIO2lowRange=0;
	m_iPRICO_FIO2highRange=0;

	m_iCurNumericBlock_IPPV=0;
	m_iCurNumericBlock_SIPPV=0;
	m_iCurNumericBlock_SIMV=0;
	m_iCurNumericBlock_SIMVPSV=0;
	m_iCurNumericBlock_PSV=0;
	m_iCurNumericBlock_CPAP=0;
	m_iCurNumericBlock_HFO=0;
	m_iCurNumericBlock_NCPAP=0;
	m_iCurNumericBlock_DUOPAP=0;
	m_iCurNumericBlock_THERAPY=0;
	m_iCurNumericBlock_FLOWOFFCONV=0;
	m_iCurNumericBlock_FLOWOFFCPAP=0;
	m_iCurNumericBlock_FLOWOFFHFO=0;
	
	m_iEthernetPort=DEFAULT_PORT;
	m_iPDMSProtocol=ACL_NOPDMS;
	m_eAcuLinkVersion=ALINKVERS_3;
	m_iSPO2module=SPO2MODULE_NONE;
	m_bySPO2ModuleTemp=m_iSPO2module;
	m_iCO2module=CO2MODULE_NONE;
	m_byCO2ModuleTemp=m_iCO2module;
	m_iMainBoard=0;

	m_iSystemHWrevision=0;
	m_eDevType=DEV_HFO;
	m_eFlowSensorType=FSENS_NEONATAL;
	m_eAVRevision=AVREV_1;
	m_eHFRevision=HFREV_1;

	m_eTubeType=TUBECFG_SELECTABLE;

	m_bFreshgasExt=false;
	/*m_iPDMSwave1=0;
	m_iPDMSwave2=0;*/

	m_bUseNeoPed=true;
	m_bUseTveBTB=true;
	m_bBTPSenable=true;
	m_iVentRange=0;
	m_bGraphAutoScale=false;
	m_bTrendAutoScale=false;
	m_bFilledGraph=false;
	m_bFilledTrend=false;
	//m_eTriggerTypeCONV=TRIGGER_FLOW;
	//m_eTriggerTypeNMODE=TRIGGER_FLOW;

	m_ePrevTrigger_CONV=TRIGGER_NONE;
	m_eTriggerType_CONV=TRIGGER_FLOW;
	m_eTriggerType_CPAP=TRIGGER_FLOW;
	m_eTriggerType_DUOPAP=TRIGGER_FLOW;
	m_eTriggerType_NCPAP=TRIGGER_FLOW;

	m_eIERelationMode=RM_SET_ItoE;
	m_eRefFlowSensor=RF_NTPD;
	m_eTubeSet=TUBE_INFANTFLOW;
	m_ePressureUnit=PUNIT_MBAR;
	m_eVControl=VCTRL_VGARANT;
	m_iPrimaryLineDiagramm=0;
	m_byFlushTime=0;

	m_iFOTconv_AMPLITUDE=0;
	m_iFOTconv_FREQ=0;
	m_iFOTconv_PEEPSTART=0;
	m_iFOTconv_PEEPEND=0;
	m_iFOTconv_STEPS=0;
	m_iDiffPEEPFOT=10;

	m_iFOThfo_AMPLITUDE=0;
	m_iFOThfo_PMEANSTART=0;
	m_iFOThfo_PMEANEND=0;
	m_iFOThfo_FREQ=0;
	m_iFOThfo_STEPS=0;
	m_iDiffPmeanFOT=10;
	
	m_iManBreathTime=0;
	m_iNebulizerTime=0;
	m_iPminAlarmDelay=0;
	m_iPatientAlarmDelay=0;
	//m_iBreathVolGaranty=0;
	m_bPlayVlimitSound=false;
	m_bPlayBackupSound=false;
	m_bShowEFlowInSettings=false;
	m_bHFOManBreathEnabled=false;
	m_bEFLOWequalILFOW=false;
	m_iPercentAbortCriterionPSV=0;
	m_eAutoOxyCal=CAL_21;
	m_eAutoScreenlocktime=ASL_30;
	m_bAutoScreenlockActive=false;
	m_eCO2unit=CO2_UNIT_MMHG;
	//m_bCO2ModuleEnabled=false;
	m_iCO2BaroPressure=0;
	m_iCO2TimePumpAutoOn=0;
	//m_iO2Compensation=0;
	/*m_bCO2FeatureAvailable=false;
	m_bVGUARANTFeatureAvailable=false;
	m_bVLIMITFeatureAvailable=false;
	m_bHFOFeatureAvailable=false;
	m_bNMODEFeatureAvailable=false;*/
	//m_iSPO2_SIQlimit=FACTORY_SPO2_SIQLIMIT;
	m_bFastSATon=false;
	m_eSPO2SensitivityMode=SPO2_SENSITIVITY_NORMAL;
	m_eSPO2alarmdelay=SPO2_ADELAY_10;
	m_eSPO2averagingTime=SPO2_AVERAGING_10;

	m_eFlowSensorState=FLOWSENSOR_ON;
	m_bServicePasswordDisabled=false;
	m_bNursecallActive=false;
	m_bEnableAutolimit=true;
	m_bEnableFactory=false;
	m_eCurMode=VM_NONE;
	m_ePrevMode=VM_NONE;
	m_eAlarmVolume=ALARMMEDIUM;
	m_iGraphSpeedHFO=0;
	m_iGraphSpeedNORM=0;
	m_iGraphSpeedCLFIO2=0;
	m_GraphLastViewState=0;
	//m_iMAXSCALE_EMG_GRAPH=0;
	//m_iMAXSCALE_CLFIO2_GRAPH=0;
	m_iMAXSCALE_SPO2_GRAPH=0;
	m_iMAXSCALE_CO2_GRAPH=0;
	m_iMAXSCALE_FLOW_GRAPH=0;
	m_iMAXSCALE_PRESSURE_GRAPH=0;
	m_iMAXSCALE_VOLUME_GRAPH=0;
	m_iMAXSCALE_SPO2_HFGRAPH=0;
	m_iMAXSCALE_CO2_HFGRAPH=0;
	m_iMAXSCALE_FLOW_HFGRAPH=0;
	m_iMAXSCALE_VOLUME_HFGRAPH=0;
	m_iMAXSCALE_PRESSURE_HFGRAPH=0;
	m_iMINSCALE_PRESSURE_HFGRAPH=0;
	m_iMAXSCALE_PRESSURE_HFPVLOOP=0;
	m_iMINSCALE_PRESSURE_HFPVLOOP=0;
	m_iMAXSCALE_VOLUME_HFPVLOOP=0;
	m_iMAXSCALE_FLOW_HFVFLOOP=0;
	m_iMAXSCALE_VOLUME_HFVFLOOP=0;
	m_iMAXSCALE_PRESSURE_PVLOOP=0;
	m_iMAXSCALE_VOLUME_PVLOOP=0;
	m_iMAXSCALE_VOLUME_VFLOOP=0;
	m_iMAXSCALE_FLOW_VFLOOP=0;
	m_iMAXSCALE_FOT_PRESSURE=0;
	m_iMINSCALE_FOT_XRS=0;
	m_iMAXSCALE_FOT_XRS=0;
	m_iTrendPInspMax=0;
	m_iTrendPMeanMax=0;
	m_iTrendFIO2Max=0;
	m_iTrendVteMax=0;
	m_iTrendComplianceMax=0;
	m_iTrendDCO2Max=0;
	m_iTrendMVMax=0;
	m_iTrendHFAMPMax=0;
	m_iTrendRSBIMax=0;
	m_iTrendSHAREMVMANDMax=0;
	m_iTrendRESISTANCEMax=0;
	m_iTrendLEAKMax=0;
	m_iTrendSPO2Max=0;
	m_iTrendSPO2PIMax=0;
	m_iTrendETCO2Max=0;
	m_iTrendSPO2PRMax=0;
	m_iTrendFREQUENCYMax=0;

	m_iTypeTrend1=0;
	m_iTypeTrend2=0;
	m_iTypeTrend3=0;
	m_iTrendSpan=0;
	m_iSetupView_SettingBtn=0;
	m_iSetupView_CO2Btn=0;
	m_iSetupView_SPO2Btn=0;
	m_iALimit_Btn=0;
	m_iFOTViewBtn=0;
	m_iParaViewBtn_IPPV=0;
	m_iParaViewBtn_PSV=0;
	m_iParaViewBtn_SIMV=0;
	m_iParaViewBtn_SIMVPSV=0;
	m_iParaViewBtn_SIPPV=0;
	m_iParaViewBtn_CPAP=0;
	m_iParaViewBtn_NCPAP=0;
	m_iParaViewBtn_DUOPAP=0;
	m_iParaViewBtn_THERAPIE=0;
	m_iParaViewBtn_HFO=0;
	m_bPPSVasDeltaPEEPValue=FALSE;
	m_eLastALimitNumeric=ALIMITNUM_VENTILATION;
	
	m_iParaDataITime_IPPV=0;
	m_iParaDataITime_TRIGGER=0;
	m_iParaDataITime_NMODE=0;
	m_iParaDataETIME_IPPV=0;
	m_iParaDataETIME_TRIGGER=0;
	m_iParaDataETIME_NMODE=0;
	m_iParaDataBPM_IPPV=0;
	m_iParaDataBPM_TRIGGER=0;
	m_iParaDataBPM_NMODE=0;
	m_iParaDataIFlow_IPPV=0;
	m_iParaDataIFlow_TRIGGER=0;
	m_iParaDataRisetime_IPPV=0;
	m_iParaDataRisetime_TRIGGER=0;
	m_iParaDataEFlow_IPPV=0;
	m_iParaDataEFlow_TRIGGER=0;
	m_iParaDataTherapieFlow=0;
	m_iParaDataFlowMin=0;
	m_iParaDataPEEP_IPPV=0;
	m_iParaDataPEEP_TRIGGER=0;
	m_iParaDataCpap=0;
	m_iParaDataNmode=0;
	m_iParaDataPInsp_IPPV=0;
	m_iParaDataPInsp_TRIGGER=0;
	m_iParaDataPmaxVolG_IPPV=0;
	m_iParaDataPmaxVolG_TRIGGER=0;
	m_iParaDataPPSV=0;
	m_iParaDataPManual_HFO=0;
	m_iParaDataPManual_CPAP=0;
	m_iParaDataPManual_NMODE=0;
	m_iParaDataO2=0;
	m_iParaDataO2Flush=0;
	m_iO2Difference=0;
	m_iPmeanDifference=0;
	m_iPPSVDifference=0;
	m_iParaDataVLimit_IPPV=0;
	m_iParaDataVLimit_TRIGGER=0;
	m_bParaDataVLimitOn_IPPV=false;
	m_bParaDataVLimitOn_TRIGGER=false;
	m_iParaDataVGarant_IPPV=0;
	m_iParaDataVGarant_TRIGGER=0;
	m_bParaDataVGarantOn_IPPV=false;
	m_bParaDataVGarantOn_TRIGGER=false;
	//m_iParaDataTriggerCONV=0;
	//m_iParaDataTriggerNMODE=0;
	m_iParaDataTrigger_CONV=0;
	m_iParaDataTrigger_CPAP=0;
	m_iParaDataTrigger_NCPAP=0;
	m_iParaDataTrigger_DUOPAP=0;
	m_iParaDataBackup=0;
	m_iParaDataHFAmpl=0;
	m_iParaDataHFAmplmax=0;
	m_iParaDataHFFreq=0;
	m_iParaDataHFPmean=0;

	m_iParaDataHFITimeRec=0;
	m_iParaDataHFFREQ_REC=0;
	m_iParaDataHFPMeanRec=0;

	m_iParaDataHFVGarant=0;
	m_bParaDataHFVGarantState=false;
	m_iParaDataHFIERatio=RIE_1_1;

	m_eCurveForm=CURVE_IFLOW;

	m_iAlarmlimitSPO2max=0;
	m_iAlarmlimitStateSPO2max=0;
	m_iAlarmlimitSPO2min=0;
	m_iAlarmlimitStateSPO2min=0;
	m_iAlarmlimitPulseRatemax=0;
	m_iAlarmlimitStatePulseRatemax=0;
	m_iAlarmlimitPulseRatemin=0;
	m_iAlarmlimitStatePulseRatemin=0;
	m_iAlarmlimitSPO2_PImin=0;
	m_iAlarmlimitStateSPO2_PImin=0;
	m_iAlarmlimitSPO2_SIQmin=0;
	m_iAlarmlimitStateSPO2_SIQmin=0;


	m_iAlarmlimitETCO2max=0;
	m_iAlarmlimitStateETCO2max=0;
	m_iAlarmlimitETCO2min=0;
	m_iAlarmlimitStateETCO2min=0;

	m_iAlarmlimitFICO2max=0;
	m_iAlarmlimitStateFICO2max=0;
	m_iAlarmlimitFICO2min=0;
	m_iAlarmlimitStateFICO2min=0;

	m_iAlarmlimitMVmax=0;
	m_iAlarmlimitStateMVmax=0;
	m_iAlarmlimitMVmin=0;
	m_iAlarmlimitStateMVmin=0;
	m_iAlarmlimitPIPmax=0;
	m_iAlarmlimitStatePIPmax=0;
	m_iAlarmlimitPIPmin=0;
	m_iAlarmlimitStatePIPmin=0;
	m_iAlarmlimitPEEPmin=0;
	m_iAlarmlimitStatePEEPmin=0;
	m_iAlarmlimitBPMmax=0;
	m_iAlarmlimitStateBPMmax=0;
	m_iAlimitLeakmax=0;
	m_iAlarmlimitStateLeakmax=0;
	m_iAlarmlimitApnoe=0;
	m_iAlarmlimitStateApnoe=0;

	m_iAlarmlimitSPO2maxHF=0;
	m_iAlarmlimitStateSPO2maxHF=0;
	m_iAlarmlimitSPO2minHF=0;
	m_iAlarmlimitStateSPO2minHF=0;

	m_iAlarmlimitPulseRatemaxHF=0;
	m_iAlarmlimitStatePulseRatemaxHF=0;
	m_iAlarmlimitPulseRateminHF=0;
	m_iAlarmlimitStatePulseRateminHF=0;

	m_iAlarmlimitSPO2_PIminHF=0;
	m_iAlarmlimitStateSPO2_PIminHF=0;

	m_iAlarmlimitSPO2_SIQminHF=0;
	m_iAlarmlimitStateSPO2_SIQminHF=0;

	m_iAlarmlimitETCO2maxHF=0;
	m_iAlarmlimitStateETCO2maxHF=0;
	m_iAlarmlimitETCO2minHF=0;
	m_iAlarmlimitStateETCO2minHF=0;

	m_iAlarmlimitFICO2maxHF=0;
	m_iAlarmlimitStateFICO2maxHF=0;
	m_iAlarmlimitFICO2minHF=0;
	m_iAlarmlimitStateFICO2minHF=0;

	m_iAlarmlimitMAPmaxHF=0;
	m_iAlarmlimitStateMAPmaxHF=0;
	m_iAlarmlimitMAPminHF=0;
	m_iAlarmlimitStateMAPminHF=0;

	m_iAlarmlimitDCO2maxHF=0;
	m_iAlarmlimitStateDCO2maxHF=0;
	m_iAlarmlimitDCO2minHF=0;
	m_iAlarmlimitStateDCO2minHF=0;
	m_iAlarmlimitMVmaxHF=0;
	m_iAlarmlimitStateMVmaxHF=0;
	m_iAlarmlimitMVminHF=0;
	m_iAlarmlimitStateMVminHF=0;
	m_iAlarmlimitPmaxHF=0;
	m_iAlarmlimitStatePmaxHF=0;
	m_iAlarmlimitPEEPminHF=0;
	m_iAlarmlimitStatePEEPminHF=0;
	m_iAlarmlimitBPMmaxHF=0;
	m_iAlarmlimitStateBPMmaxHF=0;
	m_iAlarmlimitLeakmaxHF=0;
	m_iAlarmlimitStateLeakmaxHF=0;
	m_iAlarmlimitApnoeHF=0;
	m_iAlarmlimitStateApnoeHF=0;
	m_iAlarmlimitPmaxCpap=0;
	m_iAlarmlimitStatePmaxCpap=0;
	m_iAlarmlimitPEEPminCpap=0;
	m_iAlarmlimitStatePEEPminCpap=0;

	m_iAlarmlimitPIPmaxDUOPAP=0;//rku PIPLOW DUOPAP
	m_iAlarmlimitStatePIPmaxDUOPAP=0;//rku PIPLOW DUOPAP

	m_iAlarmlimitPIPminDUOPAP=0;
	m_iAlarmlimitStatePIPminDUOPAP=0;

	m_iAlarmlimitPEEPminDUOPAP=0;
	m_iAlarmlimitStatePEEPminDUOPAP=0;

	m_iAlarmlimitPIPmaxNCPAP=0;
	m_iAlarmlimitStatePmaxNCPAP=0;
	m_iAlarmlimitPEEPminNCPAP=0;
	m_iAlarmlimitStatePEEPminNCPAP=0;

	m_eLeakCompensation=LC_MIDDLE;
	
	m_dtNextServiceDate.SetStatus(COleDateTime::null);
}

/**********************************************************************************************//**
 * Finalizes an instance of the CConfiguration class
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

CConfiguration::~CConfiguration(void)
{
	DeleteCriticalSection(&csVentMode);
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CConfiguration::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CConfiguration* CConfiguration::GetInstance()
{
	if(theConfig == NULL)
	{
		theConfig = new CConfiguration;
	}
	return theConfig;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

void CConfiguration::DestroyInstance()
{
	if(theConfig != NULL)
	{
		delete theConfig;
		theConfig = NULL;
	}
}

/**********************************************************************************************//**
 * Initializes this object
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

void CConfiguration::Init()
{
	m_pModel=NULL;

	for(int i=0;i<16;i++)
	{
		dbBufAmpCorFactor[i] = 0x0000;
	}
	dbAmpCorFactorCONV_K=10000;
	dbAmpCorFactorCONV_J=10000;
	dbAmpCorFactorHFO_K=10000;
	dbAmpCorFactorHFO_J=10000;

	m_iConfigVersion=3006;
	m_iParaDataHFFlow=0;
	m_iLanguageID=0;
	m_iFOTventDelaytime=0;
	m_bBiasFlow=false;
	m_bSPO2registry=true;
	m_bCO2registry=true;
	m_bRISETIMEregistry=true;
	
	m_iPRICO_SPO2lowRange=0;
	m_iPRICO_SPO2highRange=0;
	m_iPRICO_FIO2lowRange=0;
	m_iPRICO_FIO2highRange=0;


	m_iCurNumericBlock_IPPV=0;
	m_iCurNumericBlock_SIPPV=0;
	m_iCurNumericBlock_SIMV=0;
	m_iCurNumericBlock_SIMVPSV=0;
	m_iCurNumericBlock_PSV=0;
	m_iCurNumericBlock_CPAP=0;
	m_iCurNumericBlock_HFO=0;
	m_iCurNumericBlock_NCPAP=0;
	m_iCurNumericBlock_DUOPAP=0;
	m_iCurNumericBlock_THERAPY=0;
	m_iCurNumericBlock_FLOWOFFCONV=0;
	m_iCurNumericBlock_FLOWOFFCPAP=0;
	m_iCurNumericBlock_FLOWOFFHFO=0;
	
	m_iEthernetPort=DEFAULT_PORT;
	m_iPDMSProtocol=ACL_NOPDMS;
	m_eAcuLinkVersion=ALINKVERS_3;
	m_iSPO2module=SPO2MODULE_NONE;
	m_bySPO2ModuleTemp=m_iSPO2module;
	m_iCO2module=CO2MODULE_NONE;
	m_byCO2ModuleTemp=m_iCO2module;
	m_iMainBoard=0;
	m_iSystemHWrevision=0;
	m_eDevType=DEV_HFO;
	m_eFlowSensorType=FSENS_NEONATAL;
	m_eAVRevision=AVREV_1;
	m_eHFRevision=HFREV_1;
	m_eTubeType=TUBECFG_SELECTABLE;

	m_bFreshgasExt=false;
	/*m_iPDMSwave1=0;
	m_iPDMSwave2=0;*/

	m_bUseNeoPed=true;
	m_bUseTveBTB=true;
	m_bBTPSenable=true;
	m_iVentRange=0;
	m_bGraphAutoScale=false;
	m_bTrendAutoScale=false;
	m_bFilledGraph=false;
	m_bFilledTrend=false;
	//m_eTriggerTypeCONV=TRIGGER_FLOW;
	//m_eTriggerTypeNMODE=TRIGGER_FLOW;

	m_ePrevTrigger_CONV=TRIGGER_NONE;
	m_eTriggerType_CONV=TRIGGER_FLOW;
	m_eTriggerType_CPAP=TRIGGER_FLOW;
	m_eTriggerType_DUOPAP=TRIGGER_FLOW;
	m_eTriggerType_NCPAP=TRIGGER_FLOW;

	m_eIERelationMode=RM_SET_ItoE;
	m_eRefFlowSensor=RF_NTPD;
	m_eTubeSet=TUBE_INFANTFLOW;
	m_ePressureUnit=PUNIT_MBAR;
	m_eVControl=VCTRL_VGARANT;
	m_iPrimaryLineDiagramm=0;
	m_byFlushTime=0;

	m_iFOTconv_AMPLITUDE=0;
	m_iFOTconv_FREQ=0;
	m_iFOTconv_PEEPSTART=0;
	m_iFOTconv_PEEPEND=0;
	m_iFOTconv_STEPS=0;
	m_iDiffPEEPFOT=10;

	m_iFOThfo_AMPLITUDE=0;
	m_iFOThfo_PMEANSTART=0;
	m_iFOThfo_PMEANEND=0;
	m_iFOThfo_FREQ=0;
	m_iFOThfo_STEPS=0;
	m_iDiffPmeanFOT=10;

	m_iManBreathTime=0;
	m_iNebulizerTime=0;
	m_iPminAlarmDelay=0;
	m_iPatientAlarmDelay=0;
	//m_iBreathVolGaranty=0;
	m_bPlayVlimitSound=false;
	m_bPlayBackupSound=false;
	m_bShowEFlowInSettings=false;
	m_bHFOManBreathEnabled=false;
	m_bEFLOWequalILFOW=false;
	m_iPercentAbortCriterionPSV=0;
	m_eAutoOxyCal=CAL_21;
	m_eAutoScreenlocktime=ASL_30;
	m_bAutoScreenlockActive=false;
	m_eCO2unit=CO2_UNIT_MMHG;
	//m_bCO2ModuleEnabled=false;
	m_iCO2BaroPressure=0;
	m_iCO2TimePumpAutoOn=0;
	//m_iO2Compensation=0;
	/*m_bCO2FeatureAvailable=false;
	m_bVGUARANTFeatureAvailable=false;
	m_bVLIMITFeatureAvailable=false;
	m_bHFOFeatureAvailable=false;
	m_bNMODEFeatureAvailable=false;*/
	//m_iSPO2_SIQlimit=FACTORY_SPO2_SIQLIMIT;
	m_bFastSATon=false;
	m_eSPO2SensitivityMode=SPO2_SENSITIVITY_NORMAL;
	m_eSPO2alarmdelay=SPO2_ADELAY_10;
	m_eSPO2averagingTime=SPO2_AVERAGING_10;

	m_eFlowSensorState=FLOWSENSOR_ON;
	m_bServicePasswordDisabled=false;
	m_bNursecallActive=false;
	m_bEnableAutolimit=true;
	m_bEnableFactory=false;
	EnterCriticalSection(&csVentMode);
	m_eCurMode=VM_NONE;
	m_ePrevMode=VM_NONE;
	LeaveCriticalSection(&csVentMode);
	m_eAlarmVolume=ALARMMEDIUM;
	m_iGraphSpeedHFO=0;
	m_iGraphSpeedNORM=0;
	m_iGraphSpeedCLFIO2=0;
	m_GraphLastViewState=0;
	//m_iMAXSCALE_EMG_GRAPH=0;
	//m_iMAXSCALE_CLFIO2_GRAPH=0;
	m_iMAXSCALE_SPO2_GRAPH=0;
	m_iMAXSCALE_CO2_GRAPH=0;
	m_iMAXSCALE_FLOW_GRAPH=0;
	m_iMAXSCALE_PRESSURE_GRAPH=0;
	m_iMAXSCALE_VOLUME_GRAPH=0;
	m_iMAXSCALE_SPO2_HFGRAPH=0;
	m_iMAXSCALE_CO2_HFGRAPH=0;
	m_iMAXSCALE_FLOW_HFGRAPH=0;
	m_iMAXSCALE_VOLUME_HFGRAPH=0;
	m_iMAXSCALE_PRESSURE_HFGRAPH=0;
	m_iMINSCALE_PRESSURE_HFGRAPH=0;
	m_iMAXSCALE_PRESSURE_HFPVLOOP=0;
	m_iMINSCALE_PRESSURE_HFPVLOOP=0;
	m_iMAXSCALE_VOLUME_HFPVLOOP=0;
	m_iMAXSCALE_FLOW_HFVFLOOP=0;
	m_iMAXSCALE_VOLUME_HFVFLOOP=0;
	m_iMAXSCALE_PRESSURE_PVLOOP=0;
	m_iMAXSCALE_VOLUME_PVLOOP=0;
	m_iMAXSCALE_VOLUME_VFLOOP=0;
	m_iMAXSCALE_FLOW_VFLOOP=0;
	m_iMAXSCALE_FOT_PRESSURE=0;
	m_iMINSCALE_FOT_XRS=0;
	m_iMAXSCALE_FOT_XRS=0;
	m_iTrendPInspMax=0;
	m_iTrendPMeanMax=0;
	m_iTrendFIO2Max=0;
	m_iTrendVteMax=0;
	m_iTrendComplianceMax=0;
	m_iTrendDCO2Max=0;
	m_iTrendMVMax=0;
	m_iTrendHFAMPMax=0;
	m_iTrendRSBIMax=0;
	m_iTrendSHAREMVMANDMax=0;
	m_iTrendRESISTANCEMax=0;
	m_iTrendLEAKMax=0;
	m_iTrendSPO2Max=0;
	m_iTrendSPO2PIMax=0;
	m_iTrendETCO2Max=0;
	m_iTrendSPO2PRMax=0;
	m_iTrendFREQUENCYMax=0;

	m_iTypeTrend1=0;
	m_iTypeTrend2=0;
	m_iTypeTrend3=0;
	m_iTrendSpan=0;
	m_iSetupView_SettingBtn=0;
	m_iSetupView_CO2Btn=0;
	m_iSetupView_SPO2Btn=0;
	m_iALimit_Btn=0;
	m_iFOTViewBtn=0;
	m_iParaViewBtn_IPPV=0;
	m_iParaViewBtn_PSV=0;
	m_iParaViewBtn_SIMV=0;
	m_iParaViewBtn_SIMVPSV=0;
	m_iParaViewBtn_SIPPV=0;
	m_iParaViewBtn_CPAP=0;
	m_iParaViewBtn_NCPAP=0;
	m_iParaViewBtn_DUOPAP=0;
	m_iParaViewBtn_THERAPIE=0;
	m_iParaViewBtn_HFO=0;
	m_bPPSVasDeltaPEEPValue=FALSE;
	m_eLastALimitNumeric=ALIMITNUM_VENTILATION;
	
	m_iParaDataITime_IPPV=0;
	m_iParaDataITime_TRIGGER=0;
	m_iParaDataITime_NMODE=0;
	m_iParaDataETIME_IPPV=0;
	m_iParaDataETIME_TRIGGER=0;
	m_iParaDataETIME_NMODE=0;
	m_iParaDataBPM_IPPV=0;
	m_iParaDataBPM_TRIGGER=0;
	m_iParaDataBPM_NMODE=0;
	m_iParaDataIFlow_IPPV=0;
	m_iParaDataIFlow_TRIGGER=0;
	m_iParaDataRisetime_IPPV=0;
	m_iParaDataRisetime_TRIGGER=0;
	m_iParaDataEFlow_IPPV=0;
	m_iParaDataEFlow_TRIGGER=0;
	m_iParaDataTherapieFlow=0;
	m_iParaDataFlowMin=0;
	m_iParaDataPEEP_IPPV=0;
	m_iParaDataPEEP_TRIGGER=0;
	m_iParaDataCpap=0;
	m_iParaDataNmode=0;
	m_iParaDataPInsp_IPPV=0;
	m_iParaDataPInsp_TRIGGER=0;
	m_iParaDataPmaxVolG_IPPV=0;
	m_iParaDataPmaxVolG_TRIGGER=0;
	m_iParaDataPPSV=0;
	m_iParaDataPManual_HFO=0;
	m_iParaDataPManual_CPAP=0;
	m_iParaDataPManual_NMODE=0;
	m_iParaDataO2=0;
	m_iParaDataO2Flush=0;
	m_iO2Difference=0;
	m_iPmeanDifference=0;
	m_iPPSVDifference=0;
	m_iParaDataVLimit_IPPV=0;
	m_iParaDataVLimit_TRIGGER=0;
	m_bParaDataVLimitOn_IPPV=false;
	m_bParaDataVLimitOn_TRIGGER=false;
	m_iParaDataVGarant_IPPV=0;
	m_iParaDataVGarant_TRIGGER=0;
	m_bParaDataVGarantOn_IPPV=false;
	m_bParaDataVGarantOn_TRIGGER=false;
	//m_iParaDataTriggerCONV=0;
	//m_iParaDataTriggerNMODE=0;
	m_iParaDataTrigger_CONV=0;
	m_iParaDataTrigger_CPAP=0;
	m_iParaDataTrigger_NCPAP=0;
	m_iParaDataTrigger_DUOPAP=0;
	m_iParaDataBackup=0;
	m_iParaDataHFAmpl=0;
	m_iParaDataHFAmplmax=0;
	m_iParaDataHFFreq=0;
	m_iParaDataHFPmean=0;

	m_iParaDataHFITimeRec=0;
	m_iParaDataHFFREQ_REC=0;
	m_iParaDataHFPMeanRec=0;

	m_iParaDataHFVGarant=0;
	m_bParaDataHFVGarantState=false;
	m_iParaDataHFIERatio=RIE_1_1;

	m_eCurveForm=CURVE_IFLOW;

	m_iAlarmlimitSPO2max=0;
	m_iAlarmlimitStateSPO2max=0;
	m_iAlarmlimitSPO2min=0;
	m_iAlarmlimitStateSPO2min=0;
	m_iAlarmlimitPulseRatemax=0;
	m_iAlarmlimitStatePulseRatemax=0;
	m_iAlarmlimitPulseRatemin=0;
	m_iAlarmlimitStatePulseRatemin=0;
	m_iAlarmlimitSPO2_PImin=0;
	m_iAlarmlimitStateSPO2_PImin=0;
	m_iAlarmlimitSPO2_SIQmin=0;
	m_iAlarmlimitStateSPO2_SIQmin=0;


	m_iAlarmlimitETCO2max=0;
	m_iAlarmlimitStateETCO2max=0;
	m_iAlarmlimitETCO2min=0;
	m_iAlarmlimitStateETCO2min=0;

	m_iAlarmlimitFICO2max=0;
	m_iAlarmlimitStateFICO2max=0;
	m_iAlarmlimitFICO2min=0;
	m_iAlarmlimitStateFICO2min=0;

	m_iAlarmlimitMVmax=0;
	m_iAlarmlimitStateMVmax=0;
	m_iAlarmlimitMVmin=0;
	m_iAlarmlimitStateMVmin=0;
	m_iAlarmlimitPIPmax=0;
	m_iAlarmlimitStatePIPmax=0;
	m_iAlarmlimitPIPmin=0;
	m_iAlarmlimitStatePIPmin=0;
	m_iAlarmlimitPEEPmin=0;
	m_iAlarmlimitStatePEEPmin=0;
	m_iAlarmlimitBPMmax=0;
	m_iAlarmlimitStateBPMmax=0;
	m_iAlimitLeakmax=0;
	m_iAlarmlimitStateLeakmax=0;
	m_iAlarmlimitApnoe=0;
	m_iAlarmlimitStateApnoe=0;

	m_iAlarmlimitSPO2maxHF=0;
	m_iAlarmlimitStateSPO2maxHF=0;
	m_iAlarmlimitSPO2minHF=0;
	m_iAlarmlimitStateSPO2minHF=0;

	m_iAlarmlimitPulseRatemaxHF=0;
	m_iAlarmlimitStatePulseRatemaxHF=0;
	m_iAlarmlimitPulseRateminHF=0;
	m_iAlarmlimitStatePulseRateminHF=0;

	m_iAlarmlimitSPO2_PIminHF=0;
	m_iAlarmlimitStateSPO2_PIminHF=0;

	m_iAlarmlimitSPO2_SIQminHF=0;
	m_iAlarmlimitStateSPO2_SIQminHF=0;

	m_iAlarmlimitETCO2maxHF=0;
	m_iAlarmlimitStateETCO2maxHF=0;
	m_iAlarmlimitETCO2minHF=0;
	m_iAlarmlimitStateETCO2minHF=0;

	m_iAlarmlimitFICO2maxHF=0;
	m_iAlarmlimitStateFICO2maxHF=0;
	m_iAlarmlimitFICO2minHF=0;
	m_iAlarmlimitStateFICO2minHF=0;

	m_iAlarmlimitMAPmaxHF=0;
	m_iAlarmlimitStateMAPmaxHF=0;
	m_iAlarmlimitMAPminHF=0;
	m_iAlarmlimitStateMAPminHF=0;

	m_iAlarmlimitDCO2maxHF=0;
	m_iAlarmlimitStateDCO2maxHF=0;
	m_iAlarmlimitDCO2minHF=0;
	m_iAlarmlimitStateDCO2minHF=0;
	m_iAlarmlimitMVmaxHF=0;
	m_iAlarmlimitStateMVmaxHF=0;
	m_iAlarmlimitMVminHF=0;
	m_iAlarmlimitStateMVminHF=0;
	m_iAlarmlimitPmaxHF=0;
	m_iAlarmlimitStatePmaxHF=0;
	m_iAlarmlimitPEEPminHF=0;
	m_iAlarmlimitStatePEEPminHF=0;
	m_iAlarmlimitBPMmaxHF=0;
	m_iAlarmlimitStateBPMmaxHF=0;
	m_iAlarmlimitLeakmaxHF=0;
	m_iAlarmlimitStateLeakmaxHF=0;
	m_iAlarmlimitApnoeHF=0;
	m_iAlarmlimitStateApnoeHF=0;
	m_iAlarmlimitPmaxCpap=0;
	m_iAlarmlimitStatePmaxCpap=0;
	m_iAlarmlimitPEEPminCpap=0;
	m_iAlarmlimitStatePEEPminCpap=0;

	m_iAlarmlimitPIPmaxDUOPAP=0;//rku PIPLOW DUOPAP
	m_iAlarmlimitStatePIPmaxDUOPAP=0;//rku PIPLOW DUOPAP

	m_iAlarmlimitPIPminDUOPAP=0;
	m_iAlarmlimitStatePIPminDUOPAP=0;

	m_iAlarmlimitPEEPminDUOPAP=0;
	m_iAlarmlimitStatePEEPminDUOPAP=0;

	m_iAlarmlimitPIPmaxNCPAP=0;
	m_iAlarmlimitStatePmaxNCPAP=0;
	m_iAlarmlimitPEEPminNCPAP=0;
	m_iAlarmlimitStatePEEPminNCPAP=0;

	m_eLeakCompensation=LC_MIDDLE;
	
	m_dtNextServiceDate.SetStatus(COleDateTime::null);

	if(theConfig)
	{
		theConfig->LoadSettings();
	}
}

/**********************************************************************************************//**
 * Loads the settings
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

void CConfiguration::LoadSettings()
{
	CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);

	m_wOldConfigVersion=(WORD)regLang.ReadDWORD(_T("ConfigVersion"), 0);
	//if(m_wOldConfigVersion!=m_iConfigVersion)
	//{
	//	//do something here if needed regarding different config versions
	//	//theApp.WriteLog(_T("*** OldCOnfig ***"));
	//}
	regLang.WriteDWORD(_T("ConfigVersion"), m_iConfigVersion);

	CTlsRegistry regPlatform(_T("HKLM\\Platform"),true);
	m_szKernel=regPlatform.ReadString(_T("KernelVersion"), _T("unknown"));
	m_szKernelDate=regPlatform.ReadString(_T("KernelVersionDate"), _T("unknown"));
	m_szAcuKernel=regPlatform.ReadString(_T("AcuVersion"), _T("V3.0"));
	m_szNetDCUversion=regPlatform.ReadString(_T("BoardSubType"), _T("unknown"));

	bool bReorganize=false;
	if(m_szAcuKernel==_T("V1.0"))
	{
		bReorganize=true;
		regPlatform.WriteString(_T("AcuVersion"), _T("V3.0"));
		m_szAcuKernel=_T("V3.0");
	}

	WORD wLanguageID=(WORD)regLang.ReadDWORD(_T("LanguageID"), 0);
	m_iLanguageID=wLanguageID;
	
	if( getModel()->getLANGUAGE()->IsXMLLangFile())
		m_szLanguageID=getModel()->getLANGUAGE()->GetLangFileName(wLanguageID);
	else
	{
		theApp.WriteLog(_T("#HFO:0055"));
		switch(wLanguageID)
		{
		case LAN_ENGLISH:
			{
				m_szLanguageID=LANGFILE_ENGLISH;
			}
			break;
		case LAN_GERMAN:
			{
				m_szLanguageID=LANGFILE_GERMAN;
			}
			break;
		case LAN_RUSSIAN:
			{
				m_szLanguageID=LANGFILE_RUSSIAN;
			}
			break;
		case LAN_FRENCH:
			{
				m_szLanguageID=LANGFILE_FRENCH;
			}
			break;
		case LAN_ITALIAN:
			{
				m_szLanguageID=LANGFILE_ITALIAN;
			}
			break;
		case LAN_FINNISH:
			{
				m_szLanguageID=LANGFILE_FINNISH;
			}
			break;
		case LAN_CHINESE:
			{
				m_szLanguageID=LANGFILE_CHINESE;
			}
			break;
		case LAN_JAPANESE:
			{
				m_szLanguageID=LANGFILE_JAPANESE;
			}
			break;
		case LAN_SPANISH:
			{
				m_szLanguageID=LANGFILE_SPANISH;
			}
			break;
		case LAN_PORTUGUESE:
			{
				m_szLanguageID=LANGFILE_PORTUGUESE;
			}
			break;
		case LAN_SLOVENSKO:
			{
				m_szLanguageID=LANGFILE_SLOVENSKO;
			}
			break;
		case LAN_CZECH:
			{
				m_szLanguageID=LANGFILE_CZECH;
			}
			break;
		case LAN_SWEDISH:
			{
				m_szLanguageID=LANGFILE_SWEDISH;
			}
			break;
		case LAN_DUTCH:
			{
				m_szLanguageID=LANGFILE_DUTCH;
			}
			break;
		case LAN_POLISH:
			{
				m_szLanguageID=LANGFILE_POLISH;
			}
			break;
		case LAN_TURKISH:
			{
				m_szLanguageID=LANGFILE_TURKISH;
			}
			break;
		case LAN_AMERICAN:
			{
				m_szLanguageID=LANGFILE_AMERICAN;
			}
			break;
		case LAN_NORWAY:
			{
				m_szLanguageID=LANGFILE_NORWAY;
			}
			break;
		case LAN_HUNGARIAN:
			{
				m_szLanguageID=LANGFILE_HUNGARIAN;
			}
			break;
		case LAN_DANSK:
			{
				m_szLanguageID=LANGFILE_DANSK;
			}
			break;
		default:
			{
				m_iLanguageID=LAN_ENGLISH;
				m_szLanguageID=LANGFILE_ENGLISH;
				//getModel()->getI2C()->WriteConfigByte(LANGUAGEID_8,LAN_ENGLISH);

				/*CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
				WORD wLanguageID=regLang.ReadDWORD(_T("LanguageID"), 0);*/
				regLang.WriteDWORD(_T("LanguageID"), LAN_ENGLISH);
				regLang.FlushHKCU();

				getModel()->SetLanguageID(LAN_ENGLISH);
			}
			break;
		}
	}

	CTlsRegistry regWorkstate(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	//m_wHFamp=regWorkstate.ReadDWORD(_T("HFamplitude"), 0);

	if(regWorkstate.ReadDWORD(_T("BiasFlow"), 0)==1)
		m_bBiasFlow=true;
	else
		m_bBiasFlow=false;

	if(regWorkstate.ReadDWORD(_T("SPO2"), 0)==0)
		m_bSPO2registry=true;
	else
		m_bSPO2registry=false;

	if(regWorkstate.ReadDWORD(_T("CO2"), 0)==0)
		m_bCO2registry=true;
	else
		m_bCO2registry=false;

	readinFOTventDelaytime();

	/*if(regWorkstate.ReadDWORD(_T("PRICO"), 0)==1)
		m_bPRICO=true;
	else
		m_bPRICO=false;*/


	m_iMainBoard=getModel()->getI2C()->ReadConfigByte(HWCONF_MAINBOARD);
	/*if(m_iMainBoard==MAINBOARD3_0)
		int iSt=0;
	if(m_iMainBoard==MAINBOARD2_1)
		int iSt=0;*/
	if(m_iMainBoard>255)
	{
		getModel()->getI2C()->WriteConfigByte(HWCONF_MAINBOARD,MAINBOARD_21);
		m_iMainBoard=MAINBOARD_21;
	}
	//m_iMainBoard=MAINBOARD2_1;


	m_iSystemHWrevision=getModel()->getI2C()->ReadConfigByte(HWCONF_SYSTEM);
	if(m_iSystemHWrevision<0 || m_iSystemHWrevision>5)
	{
		getModel()->getI2C()->WriteConfigByte(HWCONF_SYSTEM,5);
		m_iSystemHWrevision=5;
	}

	m_eDevType=(eDevType)getModel()->getI2C()->ReadConfigByte(DEVICECONF);
	if(m_eDevType<DEV_EVO || m_eDevType>DEV_HFO)
	{
		getModel()->getI2C()->WriteConfigByte(DEVICECONF,DEV_HFO);
		m_eDevType=DEV_HFO;
	}

	m_eFlowSensorType=(eFlowSensorType)getModel()->getI2C()->ReadConfigByte(FLOWSENSORCONF);
	if(m_eFlowSensorType<FSENS_NEONATAL || m_eFlowSensorType>FSENS_PEDIATRIC)
	{
		getModel()->getI2C()->WriteConfigByte(FLOWSENSORCONF,FSENS_NEONATAL);
		m_eFlowSensorType=FSENS_NEONATAL;
	}

	m_eAVRevision=(eAVRevision)getModel()->getI2C()->ReadConfigByte(AVREVISION);
	if(m_eAVRevision<AVREV_1 || m_eAVRevision>AVREV_4)
	{
		getModel()->getI2C()->WriteConfigByte(AVREVISION,AVREV_1);
		m_eAVRevision=AVREV_1;
	}

	m_eHFRevision=(eHFModuleRevision)getModel()->getI2C()->ReadConfigByte(HFREVISION);
	if(m_eHFRevision<HFREV_1 || m_eHFRevision>HFREV_2)
	{
		getModel()->getI2C()->WriteConfigByte(HFREVISION,HFREV_1);
		m_eHFRevision=HFREV_1;
	}

	m_iCO2module=getModel()->getI2C()->ReadConfigByte(CO2CONFIG);
	if(m_iCO2module>CO2MODULE_MICROPOD)
	{
		getModel()->getI2C()->WriteConfigByte(CO2CONFIG,CO2MODULE_NONE);
		m_iCO2module=CO2MODULE_NONE;
	}
	if(false==m_bCO2registry)
	{
		m_iCO2module=CO2MODULE_NONE;
	}
	m_byCO2ModuleTemp=m_iCO2module;
	//TEST
	//m_iCO2module=CO2MODULE_MICROPOD;

	m_iPDMSProtocol=getModel()->getI2C()->ReadConfigByte(PDMSCONFIG);
	if(m_iPDMSProtocol>ACL_TERMINAL_WAVE)
	{
		getModel()->getI2C()->WriteConfigByte(PDMSCONFIG,ACL_NOPDMS);
		m_iPDMSProtocol=ACL_NOPDMS;
	}

	#ifdef SIMULATION_VERSION
		m_iPDMSProtocol=ACL_RS232_ASCII;
	#endif

	//TEST
	//m_iPDMSProtocol=ACL_TERMINAL_WAVE;

	m_eAcuLinkVersion=(eAcuLinkVersion)getModel()->getI2C()->ReadConfigByte(ACULINKVERSION);
	if(m_eAcuLinkVersion>ALINKVERS_4)
	{
		getModel()->getI2C()->WriteConfigByte(ACULINKVERSION,ALINKVERS_3);
		m_eAcuLinkVersion=ALINKVERS_3;
	}
	
	//TEST
	/*getModel()->getI2C()->WriteConfigByte(PDMSCONFIG,VUELINK);
	m_iPDMSProtocol=VUELINK;*/
	/*getModel()->getI2C()->WriteConfigByte(PDMSCONFIG,ACULINK);
	m_iPDMSProtocol=ACULINK;*/

	if(m_iMainBoard<MAINBOARD_30)
	{
		if(		(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE)
			&& m_iCO2module!=CO2MODULE_NONE)
		{
			getModel()->getI2C()->WriteConfigByte(CO2CONFIG,CO2MODULE_NONE);
			m_iCO2module=CO2MODULE_NONE;
			m_byCO2ModuleTemp=CO2MODULE_NONE;
			theApp.WriteLog(_T("*** CO2 disabled bvy PDMS"));
		}
	}

	m_iSPO2module=getModel()->getI2C()->ReadConfigByte(SPO2CONFIG);
	if(m_iSPO2module>SPO2MODULE_MASIMO)
	{
		getModel()->getI2C()->WriteConfigByte(SPO2CONFIG,SPO2MODULE_NONE);
		m_iSPO2module=SPO2MODULE_NONE;
	}
	if(false==m_bSPO2registry)
	{
		m_iSPO2module=SPO2MODULE_NONE;
	}
	m_bySPO2ModuleTemp=m_iSPO2module;
	//TEST
	//m_iSPO2module=SPO2MODULE_MASIMO;

	
	
	m_iEthernetPort=getModel()->getI2C()->ReadConfigWord(ETHERNETPORT_16);
	if(m_iEthernetPort<1024 || m_iEthernetPort>12000)
	{
		getModel()->getI2C()->WriteConfigWord(ETHERNETPORT_16,DEFAULT_PORT);
		m_iEthernetPort=DEFAULT_PORT;
	}
	

	if(getModel()->getI2C()->ReadConfigByte(USENEOPED_8)==0)
	{
		m_bUseNeoPed=true;
		theApp.WriteLog(_T("*** Patient range NEO+PED ***"));
	}
	else
	{
		m_bUseNeoPed=false;
		theApp.WriteLog(_T("*** Patient range NEO ***"));

		getModel()->getI2C()->WriteConfigByte(VENTRANGE_8, NEONATAL);
	}
	

	m_iVentRange=getModel()->getI2C()->ReadConfigByte(VENTRANGE_8);
	if(m_iVentRange<NEONATAL || m_iVentRange>PEDIATRIC)
	{
		m_iVentRange=NEONATAL;
		getModel()->getI2C()->WriteConfigByte(VENTRANGE_8,NEONATAL);
	}

	//TESTtrigger
	//m_iVentRange=NEONATAL;

	//disabled/hides RISETIME option, if value==1, RISETIME is disabled
	if(regWorkstate.ReadDWORD(_T("RISETIME"), 0)==0)
		m_bRISETIMEregistry=true;
	else
		m_bRISETIMEregistry=false;
	
	if(!m_bRISETIMEregistry)
	{
		m_eCurveForm=CURVE_IFLOW;
	}
	else
	{
		m_eCurveForm=(eCurveForm)getModel()->getI2C()->ReadConfigByte(PRESSURERISECTRL_8);
	}
	if(m_eCurveForm<CURVE_IFLOW || m_eCurveForm>CURVE_LINEAR)
	//if(m_eCurveForm<CURVE_IFLOW || m_eCurveForm>CURVE_AUTOFLOW)//TODO_AUTOIFLOW
	{
		m_eCurveForm=CURVE_IFLOW;
		getModel()->getI2C()->WriteConfigByte(PRESSURERISECTRL_8,0);
	}
	//if(NEONATAL==m_iVentRange && CURVE_LINEAR==m_eCurveForm)//RISE_NEO
	//{
	//	m_eCurveForm=CURVE_IFLOW;
	//	getModel()->getI2C()->WriteConfigByte(PRESSURERISECTRL_8,0);
	//}

	EnterCriticalSection(&csVentMode);//newSPI
	m_eCurMode=(eVentMode)getModel()->getI2C()->ReadConfigByte(CURVENTMODE_8);
	if(m_eCurMode<VM_IPPV || m_eCurMode>VM_THERAPIE)
	{
		m_eCurMode=VM_IPPV;
		getModel()->getI2C()->WriteConfigByte(CURVENTMODE_8,VM_IPPV);
	}

	m_ePrevMode=(eVentMode)getModel()->getI2C()->ReadConfigByte(PREVVENTMODE_8);
	if(m_ePrevMode<VM_IPPV || m_ePrevMode>VM_THERAPIE)
	{
		m_ePrevMode=VM_IPPV;
		getModel()->getI2C()->WriteConfigByte(PREVVENTMODE_8,VM_IPPV);
	}
	LeaveCriticalSection(&csVentMode);//newSPI

	m_eAlarmVolume=(eAlarmLoudness)getModel()->getI2C()->ReadConfigByte(ALARMVOLUME_8);
	if(m_eAlarmVolume!=ALARMSILENT && m_eAlarmVolume!=ALARMMEDIUM && m_eAlarmVolume!=ALARMLOUD)
	{
		m_eAlarmVolume=ALARMMEDIUM;
		getModel()->getI2C()->WriteConfigByte(ALARMVOLUME_8,ALARMMEDIUM);
	}

	/*m_iPDMSwave1=getModel()->getI2C()->ReadConfigByte(PDMS_WAVE1_8);
	if(m_iPDMSwave1<1 || m_iPDMSwave1>4)
	{
		m_iPDMSwave1=1;
		getModel()->getI2C()->WriteConfigByte(PDMS_WAVE1_8,1);
	}

	m_iPDMSwave2=getModel()->getI2C()->ReadConfigByte(PDMS_WAVE2_8);
	if(m_iPDMSwave2<1 || m_iPDMSwave2>4)
	{
		m_iPDMSwave2=2;
		getModel()->getI2C()->WriteConfigByte(PDMS_WAVE2_8,2);
	}*/

	CTlsRegistry regVolume(_T("HKCU\\ControlPanel\\Volume"),true);
	DWORD dwValue=0;
	switch(m_eAlarmVolume)
	{
	case ALARMSILENT:
		{
			//dwValue=0x33333333;
			dwValue=0x00000000;
		}
		break;
	case ALARMMEDIUM:
		{
			dwValue=0x99999999;
		}
		break;
	case ALARMLOUD:
		{
			dwValue=0xFFFFFFFF;
		}
		break;
	}
	regVolume.WriteDWORD(_T("Volume"), dwValue);

	m_iManBreathTime=getModel()->getI2C()->ReadConfigByte(MANBREATHTIME_8);
	if(m_iManBreathTime<MIN_TIME_MANBREATH || m_iManBreathTime>MAX_TIME_MANBREATH)
	{
		m_iManBreathTime=10;
		getModel()->getI2C()->WriteConfigByte(MANBREATHTIME_8,10);
	}

	m_iNebulizerTime=getModel()->getI2C()->ReadConfigByte(NEBULIZERTIME_8);
	if(m_iNebulizerTime<5 || m_iNebulizerTime>30)
	{
		m_iNebulizerTime=10;
		getModel()->getI2C()->WriteConfigByte(NEBULIZERTIME_8,10);
	}

	m_iPminAlarmDelay=getModel()->getI2C()->ReadConfigByte(PMINALARMDELAY_8);
	if(m_iPminAlarmDelay<1 || m_iPminAlarmDelay>10)
	{
		m_iPminAlarmDelay=5;
		getModel()->getI2C()->WriteConfigByte(PMINALARMDELAY_8,5);
	}

	m_iPatientAlarmDelay=getModel()->getI2C()->ReadConfigByte(PATIENTALARMDELAY_8);
	if(m_iPatientAlarmDelay>30)
	{
		m_iPatientAlarmDelay=5;
		getModel()->getI2C()->WriteConfigByte(PATIENTALARMDELAY_8,5);
	}
	

	m_byFlushTime=getModel()->getI2C()->ReadConfigByte(O2FLUSHTIME_8);
	if(m_byFlushTime!=0 && m_byFlushTime!=30 && m_byFlushTime!=60 && m_byFlushTime!=120)
	{
		getModel()->getI2C()->WriteConfigByte(O2FLUSHTIME_8,120);
		m_byFlushTime=120;
	}

	m_eRefFlowSensor=(eRefFlowSensor)getModel()->getI2C()->ReadConfigByte(REFFLOWSENSOR_8);
	if(m_eRefFlowSensor!=RF_NTPD && m_eRefFlowSensor!=RF_BTPS)
	{
		m_eRefFlowSensor=RF_NTPD;
		getModel()->getI2C()->WriteConfigByte(REFFLOWSENSOR_8,RF_NTPD);
	}

	m_ePressureUnit=(ePressureUnit)getModel()->getI2C()->ReadConfigByte(PRESSUREUNIT_8);
	if(m_ePressureUnit<PUNIT_MBAR || m_ePressureUnit>PUNIT_CMH2O)
	{
		m_ePressureUnit=PUNIT_MBAR;
		getModel()->getI2C()->WriteConfigByte(PRESSUREUNIT_8, (int)PUNIT_MBAR);
	}

	m_eVControl=(eVolumeControl)getModel()->getI2C()->ReadConfigByte(VOLUMECONTROL_8);
	if(m_eVControl<VCTRL_VLIMIT || m_ePressureUnit>VCTRL_VGARANT)
	{
		m_eVControl=VCTRL_VGARANT;
		getModel()->getI2C()->WriteConfigByte(VOLUMECONTROL_8, (int)VCTRL_VGARANT);
	}
	
	/*if(getModel()->getI2C()->ReadConfigByte(VLIMITSOUND_8)==1)
		m_bPlayVlimitSound=true;
	else*/
		m_bPlayVlimitSound=false;

	/*if(getModel()->getI2C()->ReadConfigByte(BACKUPSOUND_8)==1)
		m_bPlayBackupSound=true;
	else*/
		m_bPlayBackupSound=false;

	if(getModel()->getI2C()->ReadConfigByte(EFLOWSETTINGS_8)==1)
		m_bShowEFlowInSettings=true;
	else
		m_bShowEFlowInSettings=false;

	m_eTubeType=(eTubeConfig)getModel()->getI2C()->ReadConfigByte(TUBECONF);
	if(m_eTubeType<TUBECFG_SELECTABLE || m_eTubeType>TUBECFG_INFANTFLOW_LP)
	{
		getModel()->getI2C()->WriteConfigByte(TUBECONF,TUBECFG_SELECTABLE);
		m_eTubeType=TUBECFG_SELECTABLE;

		m_eTubeSet=(eTubeSet)getModel()->getI2C()->ReadConfigByte(TUBESET_8);
		if(m_eTubeSet<TUBE_INFANTFLOW || m_eTubeSet>TUBE_INFANTFLOW_LP)
		{
			m_eTubeSet=TUBE_INFANTFLOW;
			getModel()->getI2C()->WriteConfigByte(TUBESET_8,TUBE_INFANTFLOW);
		}
	}
	else
	{
		if(m_eTubeType==TUBECFG_INFANTFLOW)
		{
			m_eTubeSet=TUBE_INFANTFLOW;
			getModel()->getI2C()->WriteConfigByte(TUBESET_8,TUBE_INFANTFLOW);
		}
		else if(m_eTubeType==TUBECFG_MEDIJET)
		{
			m_eTubeSet=TUBE_MEDIJET;
			getModel()->getI2C()->WriteConfigByte(TUBESET_8,TUBE_MEDIJET);
		}
		else if(m_eTubeType==TUBECFG_INFANTFLOW_LP)
		{
			m_eTubeSet=TUBE_INFANTFLOW_LP;
			getModel()->getI2C()->WriteConfigByte(TUBESET_8,TUBE_INFANTFLOW_LP);
		}
		else
		{
			m_eTubeSet=(eTubeSet)getModel()->getI2C()->ReadConfigByte(TUBESET_8);
			if(m_eTubeSet<TUBE_INFANTFLOW || m_eTubeSet>TUBE_INFANTFLOW_LP)
			{
				m_eTubeSet=TUBE_INFANTFLOW;
				getModel()->getI2C()->WriteConfigByte(TUBESET_8,TUBE_INFANTFLOW);
			}
		}
	}

	

	m_eFlowSensorState=(eFlowSensorState)getModel()->getI2C()->ReadConfigByte(FLOWSENSORSTATE_8);
	if(m_eFlowSensorState<FLOWSENSOR_ON || m_eFlowSensorState>FLOWSENSOR_AUTOOFF)
	{
		m_eFlowSensorState=FLOWSENSOR_ON;
		getModel()->getI2C()->WriteConfigByte(FLOWSENSORSTATE_8,FLOWSENSOR_ON);
	}

	/*if(getModel()->getI2C()->ReadConfigByte(ASKFORPATDATA_8)==1)
		m_bAskPatDataStartup=true;
	else
		m_bAskPatDataStartup=false;*/

	/*if(getModel()->getI2C()->ReadConfigByte(VOLUMETRIGGER_8)==1)
		m_bVolumeTrigger=true;
	else
		m_bVolumeTrigger=false;*/

	m_eTriggerType_CONV=(eTriggereType)getModel()->getI2C()->ReadConfigByte(TRIGGERTYPECONV_8);
	if(m_eTriggerType_CONV<TRIGGER_VOLUME  || m_eTriggerType_CONV>TRIGGER_PRESSURE) //pressure trigger check, allowed setting: VOLUME,FLOW, PRESSURE
	{
		m_eTriggerType_CONV=TRIGGER_FLOW;
		getModel()->getI2C()->WriteConfigByte(TRIGGERTYPECONV_8,TRIGGER_FLOW);
	}

	m_ePrevTrigger_CONV=(eTriggereType)getModel()->getI2C()->ReadConfigByte(PREVTRIGGERTYPECONV_8);
	if(m_ePrevTrigger_CONV<TRIGGER_VOLUME  || m_ePrevTrigger_CONV>TRIGGER_NONE) //previous pressure trigger check, allowed setting: VOLUME,FLOW,PRESSURE,NONE
	{
		m_ePrevTrigger_CONV=TRIGGER_NONE;
		getModel()->getI2C()->WriteConfigByte(PREVTRIGGERTYPECONV_8,TRIGGER_FLOW);
	}

	m_eTriggerType_CPAP=(eTriggereType)getModel()->getI2C()->ReadConfigByte(TRIGGERTYPECPAP_8);
	if(m_eTriggerType_CPAP<TRIGGER_FLOW  || m_eTriggerType_CPAP>TRIGGER_PRESSURE) //pressure trigger check, allowed setting: FLOW, PRESSURE, OFF
	{
		m_eTriggerType_CPAP=TRIGGER_FLOW;
		getModel()->getI2C()->WriteConfigByte(m_eTriggerType_CPAP,TRIGGER_FLOW);
	}

	m_eTriggerType_NCPAP=(eTriggereType)getModel()->getI2C()->ReadConfigByte(TRIGGERTYPENCPAP_8);
	if(m_eTriggerType_NCPAP<TRIGGER_FLOW  || m_eTriggerType_NCPAP>TRIGGER_PRESSURE) //pressure trigger check, allowed setting: FLOW, PRESSURE, OFF
	{
		m_eTriggerType_NCPAP=TRIGGER_FLOW;
		getModel()->getI2C()->WriteConfigByte(TRIGGERTYPENCPAP_8,TRIGGER_FLOW);
	}

	//m_eTriggerTypeDUOPAP=(eTriggereType)getModel()->getI2C()->ReadConfigByte(TRIGGERTYPEDUOPAP_8);
	//if(m_eTriggerTypeDUOPAP<TRIGGER_FLOW  || m_eTriggerTypeDUOPAP>TRIGGER_PRESSURE) //pressure trigger check, allowed setting: FLOW, PRESSURE, OFF
	//{
	//	m_eTriggerTypeDUOPAP=TRIGGER_FLOW;
	//	getModel()->getI2C()->WriteConfigByte(TRIGGERTYPEDUOPAP_8,TRIGGER_FLOW);
	//}

	//DUOPAP trigger depends on tubeset
	//if(m_eTriggerTypeNMODE!=TRIGGER_OFF)
	{
		if(m_eTubeSet==TUBE_INFANTFLOW)
			m_eTriggerType_DUOPAP=TRIGGER_FLOW;
		else if(m_eTubeSet==TUBE_INFANTFLOW_LP)
			m_eTriggerType_DUOPAP=TRIGGER_FLOW;
		else if(m_eTubeSet==TUBE_MEDIJET)
			m_eTriggerType_DUOPAP=TRIGGER_PRESSURE;
		/*else
			m_eTriggerTypeNMODE=TRIGGER_OFF;*/

		getModel()->getI2C()->WriteConfigByte(TRIGGERTYPEDUOPAP_8,m_eTriggerType_DUOPAP);
	}

	m_eAutoOxyCal=(eAutoOxyCal)getModel()->getI2C()->ReadConfigByte(AUTOOXYCAL_8);

	if(getModel()->getI2C()->ReadConfigByte(AUTOSCREENLOCKSTATE_8)==1)
	{
		m_bAutoScreenlockActive=true;
		theApp.WriteLog(_T("*** AutoScreenlock enabled"));
	}
	else
	{
		m_bAutoScreenlockActive=false;
		theApp.WriteLog(_T("*** AutoScreenlock disabled"));
	}

	m_eAutoScreenlocktime=(eAutoScreenlock)getModel()->getI2C()->ReadConfigByte(AUTOSCREENLOCKTIME_8);
	if(m_eAutoScreenlocktime<ASL_20 || m_eAutoScreenlocktime>ASL_60)
	{
		m_eAutoScreenlocktime=ASL_60;
		getModel()->getI2C()->WriteConfigByte(AUTOSCREENLOCKTIME_8, (BYTE)m_eAutoScreenlocktime);
	}
	if(true==m_bAutoScreenlockActive)
	{
		switch(m_eAutoScreenlocktime)
		{
		case ASL_20:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_20"));
			}
			break;
		case ASL_30:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_30"));
			}
			break;
		case ASL_40:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_40"));
			}
			break;
		case ASL_50:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_50"));
			}
			break;
		case ASL_60:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_60"));
			}
			break;
		}
	}
	

	

	theApp.SetAutoScreenlockActive(m_bAutoScreenlockActive);

	if(getModel()->getI2C()->ReadConfigByte(SERVICEPSWRDDISABLED_8)==1)
	{
		m_bServicePasswordDisabled=true;
		theApp.WriteLog(_T("*** ServicePasswordDisabled==true"));
	}
	else
	{
		m_bServicePasswordDisabled=false;
		theApp.WriteLog(_T("*** ServicePasswordDisabled==false"));
	}

	if(getModel()->getI2C()->ReadConfigByte(NURSECALLACTIVE_8)==1)
	{
		m_bNursecallActive=true;
		theApp.WriteLog(_T("*** NurscallActive==true"));
	}
	else
	{
		m_bNursecallActive=false;
		theApp.WriteLog(_T("*** NurscallActive==false"));
	}

	if(getModel()->getI2C()->ReadConfigByte(AUTOALARMLIMIT_8)==0)
	{
		m_bEnableAutolimit=true;
		theApp.WriteLog(_T("*** EnableAutolimit==true"));
	}
	else
	{
		m_bEnableAutolimit=false;
		theApp.WriteLog(_T("*** EnableAutolimit==false"));
	}

	if(bReorganize)
	{
		//check old config default values and merge to new default settings
		//old config (default: factory==0==true)
		if(getModel()->getI2C()->ReadConfigByte(FACTORYSETTINGS_8)==0)
			m_bEnableFactory=true;
		else
			m_bEnableFactory=false;

		//change to new default values
		//new config (default: factory==1==true)
		if(m_bEnableFactory)
			getModel()->getI2C()->WriteConfigByte(FACTORYSETTINGS_8, 1);
		else
			getModel()->getI2C()->WriteConfigByte(FACTORYSETTINGS_8, 0);

		//old config (default: UseTveBTB==1==true)
		if(getModel()->getI2C()->ReadConfigByte(USETVEBTB_8)==1)
		{
			m_bUseTveBTB=true;
			theApp.WriteLog(_T("*** UseTveBTB==true ***"));
		}
		else
		{
			m_bUseTveBTB=false;
			theApp.WriteLog(_T("*** UseTveBTB==false ***"));
		}

		//new config (default: UseTveBTB==0==true)
		if(m_bUseTveBTB)
			getModel()->getI2C()->WriteConfigByte(USETVEBTB_8, 0);
		else
			getModel()->getI2C()->WriteConfigByte(USETVEBTB_8, 1);
	}
	else
	{
		if(getModel()->getI2C()->ReadConfigByte(FACTORYSETTINGS_8)==0)
			m_bEnableFactory=false;
		else
			m_bEnableFactory=true;

		if(getModel()->getI2C()->ReadConfigByte(USETVEBTB_8)==0)
		{
			m_bUseTveBTB=true;
			theApp.WriteLog(_T("*** UseTveBTB==true ***"));
		}
		else
		{
			m_bUseTveBTB=false;
			theApp.WriteLog(_T("*** UseTveBTB==false ***"));
		}
	}


	

	

	/*if(getModel()->getI2C()->ReadConfigByte(PLAYTRIGSOUND_8)==1)
		m_bPlayTriggerSound=true;
	else
		m_bPlayTriggerSound=false;*/

	if(getModel()->getI2C()->ReadConfigByte(HFOMANBREATH_8)==1)
		m_bHFOManBreathEnabled=true;
	else
		m_bHFOManBreathEnabled=false;

	if(getModel()->getI2C()->ReadConfigByte(EFLOWEQUIFLOW_8)==1)
		m_bEFLOWequalILFOW=true;
	else
		m_bEFLOWequalILFOW=false;
	

	/*m_iBreathVolGaranty=getModel()->getI2C()->ReadConfigByte(BREATHVOLGARANTY_8);
	if(m_iBreathVolGaranty<2 || m_iBreathVolGaranty>10)
	{
		m_iBreathVolGaranty=5;
		getModel()->getI2C()->WriteConfigByte(BREATHVOLGARANTY_8,m_iBreathVolGaranty);
	}*/

	m_iPercentAbortCriterionPSV=getModel()->getI2C()->ReadConfigByte(ABORTCRITERIONPSV_8);
	if(m_iPercentAbortCriterionPSV<MIN_ABORTCRITERION_PSV || m_iPercentAbortCriterionPSV>MAX_ABORTCRITERION_PSV)
	{
		m_iPercentAbortCriterionPSV=25;
		getModel()->getI2C()->WriteConfigByte(ABORTCRITERIONPSV_8,m_iPercentAbortCriterionPSV);
	}

	if(getModel()->getI2C()->ReadConfigByte(FILLEDGRAPH_8)==1)
		m_bFilledGraph=true;
	else
		m_bFilledGraph=false;

	if(getModel()->getI2C()->ReadConfigByte(FILLEDTREND_8)==1)
		m_bFilledTrend=true;
	else
		m_bFilledTrend=false;

	if(getModel()->getI2C()->ReadConfigByte(BTPS_8)==1)
	{
		m_bBTPSenable=true;
		theApp.WriteLog(_T("*** BTPS enabled"));
	}
	else
	{
		m_bBTPSenable=false;
		theApp.WriteLog(_T("*** BTPS disabled"));
	}

	if(getModel()->getI2C()->ReadConfigByte(FRESHGASEXT_8)==1)
	{
		m_bFreshgasExt=true;
		theApp.WriteLog(_T("*** BiasFlow ext"));
	}
	else
	{
		m_bFreshgasExt=false;
		theApp.WriteLog(_T("*** BiasFlow int"));
	}

	if(getModel()->getI2C()->ReadConfigByte(GRAPHAUTOSCALE_8)==1)
		m_bGraphAutoScale=true;
	else
		m_bGraphAutoScale=false;

	if(getModel()->getI2C()->ReadConfigByte(TRENDAUTOSCALE_8)==1)
		m_bTrendAutoScale=true;
	else
		m_bTrendAutoScale=false;

	m_GraphLastViewState=getModel()->getI2C()->ReadConfigByte(GRAPHLASTVIEWSUBSTATE_8);
	if(m_GraphLastViewState<VSS_GRAPH_SINGLE_LINEGRAPHS || m_GraphLastViewState>VSS_GRAPH_FOTGRAPHS)
	{
		m_GraphLastViewState=2;
		getModel()->getI2C()->WriteConfigByte(GRAPHLASTVIEWSUBSTATE_8,2);
	}

	m_eLastALimitNumeric=(eALimitNumeric)getModel()->getI2C()->ReadConfigByte(ALIMITLASTNUMERICSTATE_8);
	if(m_eLastALimitNumeric<ALIMITNUM_VENTILATION || m_eLastALimitNumeric>ALIMITNUM_SPO2)
	{
		m_eLastALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getI2C()->WriteConfigByte(ALIMITLASTNUMERICSTATE_8,ALIMITNUM_VENTILATION);
	}

	m_iGraphSpeedCLFIO2=getModel()->getI2C()->ReadConfigByte(GRAPHSPEEDCLFIO2_8);
	if(m_iGraphSpeedCLFIO2<G_SPEED_99500MS || m_iGraphSpeedCLFIO2>G_SPEED_796000MS)
	{
		m_iGraphSpeedCLFIO2=G_SPEED_9950MS;
		getModel()->getI2C()->WriteConfigByte(GRAPHSPEEDCLFIO2_8,G_SPEED_9950MS);
	}

	m_iGraphSpeedNORM=getModel()->getI2C()->ReadConfigByte(GRAPHSPEEDNORM_8);
	if(m_iGraphSpeedNORM<G_SPEED_505MS || m_iGraphSpeedNORM>G_SPEED_49750MS)
	{
		m_iGraphSpeedNORM=G_SPEED_9950MS;
		getModel()->getI2C()->WriteConfigByte(GRAPHSPEEDNORM_8,G_SPEED_9950MS);
	}
	m_iGraphSpeedHFO=getModel()->getI2C()->ReadConfigByte(GRAPHSPEEDHFO_8);
	if(m_iGraphSpeedHFO<G_SPEED_505MS || m_iGraphSpeedHFO>G_SPEED_49750MS)
	{
		m_iGraphSpeedHFO=G_SPEED_1000MS;
		getModel()->getI2C()->WriteConfigByte(GRAPHSPEEDHFO_8,G_SPEED_1000MS);
	}
#ifdef SIMULATION_VERSION
	m_iGraphSpeedNORM=G_SPEED_9950MS;
	m_iGraphSpeedHFO=G_SPEED_1000MS;
#endif

	m_iPrimaryLineDiagramm=getModel()->getI2C()->ReadConfigByte(CURLINEDIAGRAMM_8);
	if(m_iPrimaryLineDiagramm<IDC_LINEDIAGRAM || m_iPrimaryLineDiagramm>IDC_LINEDIAGRAM_VOLUME)
	{
		m_iPrimaryLineDiagramm=IDC_LINEDIAGRAM;
		getModel()->getI2C()->WriteConfigByte(CURLINEDIAGRAMM_8,IDC_LINEDIAGRAM);
	}

	m_iTypeTrend1=getModel()->getI2C()->ReadConfigByte(TREND_TYPE1_8);
	if(m_iTypeTrend1<TREND_PINSP || m_iTypeTrend1>TREND_FREQUENCY)
	{
		m_iTypeTrend1=TREND_PINSP;
		getModel()->getI2C()->WriteConfigByte(TREND_TYPE1_8,TREND_PINSP);
	}

	m_iTypeTrend2=getModel()->getI2C()->ReadConfigByte(TREND_TYPE2_8);
	if(m_iTypeTrend2<TREND_PINSP || m_iTypeTrend2>TREND_FREQUENCY)
	{
		m_iTypeTrend2=TREND_PMEAN;
		getModel()->getI2C()->WriteConfigByte(TREND_TYPE2_8,TREND_PMEAN);
	}

	m_iTypeTrend3=getModel()->getI2C()->ReadConfigByte(TREND_TYPE3_8);
	if(m_iTypeTrend3<TREND_PINSP || m_iTypeTrend3>TREND_FREQUENCY)
	{
		m_iTypeTrend3=TREND_FIO2;
		getModel()->getI2C()->WriteConfigByte(TREND_TYPE3_8,TREND_FIO2);
	}

	m_bPPSVasDeltaPEEPValue=getModel()->getI2C()->ReadConfigByte(PPSV_AS_PEEPDELTA_8);
	if(m_bPPSVasDeltaPEEPValue<FALSE || m_bPPSVasDeltaPEEPValue>TRUE)
	{
		m_bPPSVasDeltaPEEPValue=FALSE;
		getModel()->getI2C()->WriteConfigByte(PPSV_AS_PEEPDELTA_8,FALSE);
	}

	m_iTrendSpan=getModel()->getI2C()->ReadConfigWord(TREND_SPAN_16);
	if(		m_iTrendSpan!=TREND_30MIN 
		&&	m_iTrendSpan!=TREND_1HOUR
		&&	m_iTrendSpan!=TREND_2HOURS
		&&	m_iTrendSpan!=TREND_4HOURS
		&&	m_iTrendSpan!=TREND_8HOURS
		&&	m_iTrendSpan!=TREND_16HOURS
		&&	m_iTrendSpan!=TREND_1DAY4HOURS
		&&	m_iTrendSpan!=TREND_3DAYS
		&&	m_iTrendSpan!=TREND_5DAYS
		/*&&	m_iTrendSpan!=TREND_10DAYS*/)
	{
		m_iTrendSpan=TREND_16HOURS;
		getModel()->getI2C()->WriteConfigWord(TREND_SPAN_16,TREND_16HOURS);
	}

	m_iTrendPInspMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_PINSP_16);
	if(m_iTrendPInspMax<G_LOWER_MAXSCALE_PRESSURE || m_iTrendPInspMax>G_UPPER_MAXSCALE_PRESSURE)
	{
		m_iTrendPInspMax=G_UPPER_MAXSCALE_PRESSURE;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_PINSP_16,G_UPPER_MAXSCALE_PRESSURE);
	}
	m_iTrendPMeanMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_PMEAN_16);
	if(m_iTrendPMeanMax<G_LOWER_MAXSCALE_PRESSURE || m_iTrendPMeanMax>G_UPPER_MAXSCALE_PRESSURE)
	{
		m_iTrendPMeanMax=G_UPPER_MAXSCALE_PRESSURE;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_PMEAN_16,m_iTrendPMeanMax);
	}
	m_iTrendFIO2Max=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_FIO2_16);
	if(m_iTrendFIO2Max<G_LOWER_MAXSCALE_FIO2 || m_iTrendFIO2Max>G_UPPER_MAXSCALE_FIO2)
	{
		m_iTrendFIO2Max=G_UPPER_MAXSCALE_FIO2;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_FIO2_16,m_iTrendFIO2Max);
	}
	m_iTrendVteMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_VTE_16);
	if(m_iTrendVteMax<G_LOWER_MAXSCALE_VTE || m_iTrendVteMax>G_UPPER_MAXSCALE_VTE)
	{
		m_iTrendVteMax=G_UPPER_MAXSCALE_VTE;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_VTE_16,m_iTrendVteMax);
	}
	m_iTrendComplianceMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_COMPLIANCE_16);
	if(m_iTrendComplianceMax<G_LOWER_MAXSCALE_COMPL || m_iTrendComplianceMax>G_UPPER_MAXSCALE_COMPL)
	{
		m_iTrendComplianceMax=G_UPPER_MAXSCALE_COMPL;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_COMPLIANCE_16,m_iTrendComplianceMax);
	}
	m_iTrendDCO2Max=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_DCO2_16);
	if(m_iTrendDCO2Max<G_LOWER_MAXSCALE_DCO2 || m_iTrendDCO2Max>G_UPPER_MAXSCALE_DCO2)
	{
		m_iTrendDCO2Max=G_UPPER_MAXSCALE_DCO2;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_DCO2_16,m_iTrendDCO2Max);
	}
	m_iTrendMVMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_MV_16);
	if(m_iTrendMVMax<G_LOWER_MAXSCALE_MV || m_iTrendMVMax>G_UPPER_MAXSCALE_MV)
	{
		m_iTrendMVMax=G_UPPER_MAXSCALE_MV;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_MV_16,m_iTrendMVMax);
	}

	m_iTrendHFAMPMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_HFAMP_16);
	if(m_iTrendHFAMPMax<G_LOWER_MAXSCALE_HFAMP || m_iTrendHFAMPMax>G_UPPER_MAXSCALE_HFAMP)
	{
		m_iTrendHFAMPMax=G_UPPER_MAXSCALE_HFAMP;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_HFAMP_16,m_iTrendHFAMPMax);
	}

	m_iTrendRSBIMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_RSBI_16);
	if(m_iTrendRSBIMax<G_LOWER_MAXSCALE_RSBI || m_iTrendRSBIMax>G_UPPER_MAXSCALE_RSBI)
	{
		m_iTrendRSBIMax=G_UPPER_MAXSCALE_RSBI;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_RSBI_16,m_iTrendRSBIMax);
	}

	m_iTrendSHAREMVMANDMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_SHAREMVMAND_16);
	if(m_iTrendSHAREMVMANDMax<G_LOWER_MAXSCALE_SHAREMVMAND || m_iTrendSHAREMVMANDMax>G_UPPER_MAXSCALE_SHAREMVMAND)
	{
		m_iTrendSHAREMVMANDMax=G_UPPER_MAXSCALE_SHAREMVMAND;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SHAREMVMAND_16,m_iTrendSHAREMVMANDMax);
	}

	m_iTrendRESISTANCEMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_RESISTANCE_16);
	if(m_iTrendRESISTANCEMax<G_LOWER_MAXSCALE_RESISTANCE || m_iTrendRESISTANCEMax>G_UPPER_MAXSCALE_RESISTANCE)
	{
		m_iTrendRESISTANCEMax=G_UPPER_MAXSCALE_RESISTANCE;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_RESISTANCE_16,m_iTrendRESISTANCEMax);
	}

	m_iTrendLEAKMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_LEAK_16);
	if(m_iTrendLEAKMax<G_LOWER_MAXSCALE_LEAK || m_iTrendLEAKMax>G_UPPER_MAXSCALE_LEAK)
	{
		m_iTrendLEAKMax=G_UPPER_MAXSCALE_LEAK;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_LEAK_16,m_iTrendLEAKMax);
	}

	m_iTrendSPO2Max=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_SPO2_16);
	if(m_iTrendSPO2Max<G_LOWER_MAXSCALE_SPO2 || m_iTrendSPO2Max>G_UPPER_MAXSCALE_SPO2)
	{
		m_iTrendSPO2Max=G_UPPER_MAXSCALE_SPO2;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SPO2_16,m_iTrendSPO2Max);
	}

	m_iTrendSPO2PIMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_SPO2PI_16);
	if(m_iTrendSPO2PIMax<G_LOWER_MAXSCALE_SPO2PI || m_iTrendSPO2PIMax>G_UPPER_MAXSCALE_SPO2PI)
	{
		m_iTrendSPO2PIMax=G_UPPER_MAXSCALE_SPO2PI;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SPO2PI_16,m_iTrendSPO2PIMax);
	}

	m_iTrendETCO2Max=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_ETCO2_16);
	if(m_iTrendETCO2Max<G_LOWER_MAXSCALE_CO2 || m_iTrendETCO2Max>G_UPPER_MAXSCALE_CO2)
	{
		m_iTrendETCO2Max=G_UPPER_MAXSCALE_CO2;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_ETCO2_16,m_iTrendETCO2Max);
	}

	m_iTrendSPO2PRMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_SPO2PR_16);
	if(m_iTrendSPO2PRMax<G_LOWER_MAXSCALE_SPO2PR || m_iTrendSPO2PRMax>G_UPPER_MAXSCALE_SPO2PR)
	{
		m_iTrendSPO2PRMax=G_UPPER_MAXSCALE_SPO2PR;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SPO2PR_16,m_iTrendSPO2PRMax);
	}

	m_iTrendFREQUENCYMax=getModel()->getI2C()->ReadConfigWord(TRENDSCALE_FREQUENCY_16);
	if(m_iTrendFREQUENCYMax<G_LOWER_MAXSCALE_FREQUENCY || m_iTrendFREQUENCYMax>G_UPPER_MAXSCALE_FREQUENCY)
	{
		m_iTrendFREQUENCYMax=G_UPPER_MAXSCALE_FREQUENCY;
		getModel()->getI2C()->WriteConfigWord(TRENDSCALE_FREQUENCY_16,m_iTrendFREQUENCYMax);
	}

	m_iMAXSCALE_SPO2_GRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_SPO2_GRAPH_16);
	if(m_iMAXSCALE_SPO2_GRAPH<G_LOWER_MAXSCALE_SPO2*10 || m_iMAXSCALE_SPO2_GRAPH>G_UPPER_MAXSCALE_SPO2*10)
	{
		m_iMAXSCALE_SPO2_GRAPH=G_UPPER_MAXSCALE_SPO2*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_SPO2_GRAPH_16,G_UPPER_MAXSCALE_SPO2*10);
	}

	m_iMAXSCALE_CO2_GRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_CO2_GRAPH_16);
	if(m_iMAXSCALE_CO2_GRAPH<G_LOWER_MAXSCALE_CO2*10 || m_iMAXSCALE_CO2_GRAPH>G_UPPER_MAXSCALE_CO2*10)
	{
		m_iMAXSCALE_CO2_GRAPH=G_UPPER_MAXSCALE_CO2*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_CO2_GRAPH_16,G_UPPER_MAXSCALE_CO2*10);
	}
	m_iMAXSCALE_FLOW_GRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_FLOW_GRAPH_16);
	if(m_iMAXSCALE_FLOW_GRAPH<G_LOWER_MAXSCALE_FLOW*10 || m_iMAXSCALE_FLOW_GRAPH>G_UPPER_MAXSCALE_FLOW*10)
	{
		m_iMAXSCALE_FLOW_GRAPH=G_UPPER_MAXSCALE_FLOW*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_GRAPH_16,G_UPPER_MAXSCALE_FLOW*10);
	}
	m_iMAXSCALE_PRESSURE_GRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_PRESSURE_GRAPH_16);
	
	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		if(m_iMAXSCALE_PRESSURE_GRAPH<G_LOWER_MAXSCALE_PRESSURE*10 || m_iMAXSCALE_PRESSURE_GRAPH>G_UPPER_MAXSCALE_NMODEPRESSURE*10)
		{
			m_iMAXSCALE_PRESSURE_GRAPH=G_UPPER_MAXSCALE_NMODEPRESSURE*10;
			getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_GRAPH_16,G_UPPER_MAXSCALE_NMODEPRESSURE*10);
		}
	}
	else
	{
		if(m_iMAXSCALE_PRESSURE_GRAPH<G_LOWER_MAXSCALE_PRESSURE*10 || m_iMAXSCALE_PRESSURE_GRAPH>G_UPPER_MAXSCALE_PRESSURE*10)
		{
			m_iMAXSCALE_PRESSURE_GRAPH=G_UPPER_MAXSCALE_PRESSURE*10;
			getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_GRAPH_16,G_UPPER_MAXSCALE_PRESSURE*10);
		}
	}

	int iUPPER_MAXSCALE_VOLUME=0;
	if(m_iVentRange==NEONATAL)
		iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
	else
		iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

	m_iMAXSCALE_VOLUME_GRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_VOLUME_GRAPH_16);
	if(m_iMAXSCALE_VOLUME_GRAPH<G_LOWER_MAXSCALE_VOLUME*10 || m_iMAXSCALE_VOLUME_GRAPH>iUPPER_MAXSCALE_VOLUME*10)
	{
		m_iMAXSCALE_VOLUME_GRAPH=iUPPER_MAXSCALE_VOLUME*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_GRAPH_16,iUPPER_MAXSCALE_VOLUME*10);
	}

	m_iMAXSCALE_SPO2_HFGRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_SPO2_HFGRAPH_16);
	if(m_iMAXSCALE_SPO2_HFGRAPH<G_LOWER_MAXSCALE_SPO2*10 || m_iMAXSCALE_SPO2_HFGRAPH>G_UPPER_MAXSCALE_SPO2*10)
	{
		m_iMAXSCALE_SPO2_HFGRAPH=G_UPPER_MAXSCALE_SPO2*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_SPO2_HFGRAPH_16,G_UPPER_MAXSCALE_SPO2*10);
	}

	m_iMAXSCALE_CO2_HFGRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_CO2_HFGRAPH_16);
	if(m_iMAXSCALE_CO2_HFGRAPH<G_LOWER_MAXSCALE_CO2*10 || m_iMAXSCALE_CO2_HFGRAPH>G_UPPER_MAXSCALE_CO2*10)
	{
		m_iMAXSCALE_CO2_HFGRAPH=G_UPPER_MAXSCALE_CO2*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_CO2_HFGRAPH_16,G_UPPER_MAXSCALE_CO2*10);
	}

	m_iMAXSCALE_FLOW_HFGRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_FLOW_HFGRAPH_16);
	if(m_iMAXSCALE_FLOW_HFGRAPH<G_LOWER_MAXSCALE_FLOW*10 || m_iMAXSCALE_FLOW_HFGRAPH>G_UPPER_MAXSCALE_FLOW*10)
	{
		m_iMAXSCALE_FLOW_HFGRAPH=G_UPPER_MAXSCALE_FLOW*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_HFGRAPH_16,G_UPPER_MAXSCALE_FLOW*10);
	}
	m_iMAXSCALE_VOLUME_HFGRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_VOLUME_HFGRAPH_16);
	if(m_iMAXSCALE_VOLUME_HFGRAPH<G_LOWER_MAXSCALE_VOLUME_HF*10 || m_iMAXSCALE_VOLUME_HFGRAPH>G_UPPER_MAXSCALE_VOLUME_HF*10)
	{
		m_iMAXSCALE_VOLUME_HFGRAPH=G_UPPER_MAXSCALE_VOLUME_HF*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_HFGRAPH_16,G_UPPER_MAXSCALE_VOLUME_HF*10);
	}
	m_iMAXSCALE_PRESSURE_HFGRAPH=getModel()->getI2C()->ReadConfigWord(MAXSCALE_PRESSURE_HFGRAPH_16);
	if(m_iMAXSCALE_PRESSURE_HFGRAPH<G_LOWER_MAXSCALE_PRESSUREHF*10 || m_iMAXSCALE_PRESSURE_HFGRAPH>G_UPPER_MAXSCALE_PRESSUREHF*10)
	{
		m_iMAXSCALE_PRESSURE_HFGRAPH=G_UPPER_MAXSCALE_PRESSUREHF*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_HFGRAPH_16,G_UPPER_MAXSCALE_PRESSUREHF*10);
	}
	m_iMINSCALE_PRESSURE_HFGRAPH=getModel()->getI2C()->ReadConfigWord(MINSCALE_PRESSURE_HFGRAPH_16);
	if(m_iMINSCALE_PRESSURE_HFGRAPH<G_LOWER_MINSCALE_PRESSUREHF*10)
	{
		m_iMINSCALE_PRESSURE_HFGRAPH=G_LOWER_MINSCALE_PRESSUREHF*10;
		getModel()->getI2C()->WriteConfigWord(MINSCALE_PRESSURE_HFGRAPH_16,G_LOWER_MINSCALE_PRESSUREHF*10);
	}
	else if(m_iMINSCALE_PRESSURE_HFGRAPH>G_UPPER_MINSCALE_PRESSUREHF*10)
	{
		m_iMINSCALE_PRESSURE_HFGRAPH=G_UPPER_MINSCALE_PRESSUREHF*10;
		getModel()->getI2C()->WriteConfigWord(MINSCALE_PRESSURE_HFGRAPH_16,G_UPPER_MINSCALE_PRESSUREHF*10);
	}

	m_iMAXSCALE_PRESSURE_HFPVLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_PRESSURE_HFPVLOOP_16);
	if(m_iMAXSCALE_PRESSURE_HFPVLOOP<G_LOWER_MAXSCALE_PRESSUREHF*10 || m_iMAXSCALE_PRESSURE_HFPVLOOP>G_UPPER_MAXSCALE_PRESSUREHF*10)
	{
		m_iMAXSCALE_PRESSURE_HFPVLOOP=G_UPPER_MAXSCALE_PRESSUREHF*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_HFPVLOOP_16,G_UPPER_MAXSCALE_PRESSUREHF*10);
	}
	m_iMINSCALE_PRESSURE_HFPVLOOP=getModel()->getI2C()->ReadConfigWord(MINSCALE_PRESSURE_HFPVLOOP_16);
	if(m_iMINSCALE_PRESSURE_HFPVLOOP<G_LOWER_MINSCALE_PRESSUREHF*10 || m_iMINSCALE_PRESSURE_HFPVLOOP>G_UPPER_MINSCALE_PRESSUREHF*10)
	{
		m_iMINSCALE_PRESSURE_HFPVLOOP=G_UPPER_MINSCALE_PRESSUREHF*10;
		getModel()->getI2C()->WriteConfigWord(MINSCALE_PRESSURE_HFPVLOOP_16,G_UPPER_MINSCALE_PRESSUREHF*10);
	}
	m_iMAXSCALE_VOLUME_HFPVLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_VOLUME_HFPVLOOP_16);
	if(m_iMAXSCALE_VOLUME_HFPVLOOP<G_LOWER_MAXSCALE_VOLUME_HF*10 || m_iMAXSCALE_VOLUME_HFPVLOOP>G_UPPER_MAXSCALE_VOLUME_HF*10)
	{
		m_iMAXSCALE_VOLUME_HFPVLOOP=G_UPPER_MAXSCALE_VOLUME_HF*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_HFPVLOOP_16,G_UPPER_MAXSCALE_VOLUME_HF*10);
	}
	m_iMAXSCALE_FLOW_HFVFLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_FLOW_HFVFLOOP_16);
	if(m_iMAXSCALE_FLOW_HFVFLOOP<G_LOWER_MAXSCALE_FLOW*10 || m_iMAXSCALE_FLOW_HFVFLOOP>G_UPPER_MAXSCALE_FLOW*10)
	{
		m_iMAXSCALE_FLOW_HFVFLOOP=G_UPPER_MAXSCALE_FLOW*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_HFVFLOOP_16,G_UPPER_MAXSCALE_FLOW*10);
	}
	m_iMAXSCALE_VOLUME_HFVFLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_VOLUME_HFVFLOOP_16);
	if(m_iMAXSCALE_VOLUME_HFVFLOOP<G_LOWER_MAXSCALE_VOLUME_HF*10 || m_iMAXSCALE_VOLUME_HFVFLOOP>G_UPPER_MAXSCALE_VOLUME_HF*10)
	{
		m_iMAXSCALE_VOLUME_HFVFLOOP=G_UPPER_MAXSCALE_VOLUME_HF*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_HFVFLOOP_16,G_UPPER_MAXSCALE_VOLUME_HF*10);
	}

	m_iMAXSCALE_PRESSURE_PVLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_PRESSURE_PVLOOP_16);
	if(m_iMAXSCALE_PRESSURE_PVLOOP<G_LOWER_MAXSCALE_PRESSURE*10 || m_iMAXSCALE_PRESSURE_PVLOOP>G_UPPER_MAXSCALE_PRESSURE*10)
	{
		m_iMAXSCALE_PRESSURE_PVLOOP=G_UPPER_MAXSCALE_PRESSURE*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_PVLOOP_16,G_UPPER_MAXSCALE_PRESSURE*10);
	}
	m_iMAXSCALE_VOLUME_PVLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_VOLUME_PVLOOP_16);
	if(m_iMAXSCALE_VOLUME_PVLOOP<G_LOWER_MAXSCALE_VOLUME*10 || m_iMAXSCALE_VOLUME_PVLOOP>iUPPER_MAXSCALE_VOLUME*10)
	{
		m_iMAXSCALE_VOLUME_PVLOOP=iUPPER_MAXSCALE_VOLUME*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_PVLOOP_16,iUPPER_MAXSCALE_VOLUME*10);
	}
	m_iMAXSCALE_VOLUME_VFLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_VOLUME_VFLOOP_16);
	if(m_iMAXSCALE_VOLUME_VFLOOP<G_LOWER_MAXSCALE_VOLUME*10 || m_iMAXSCALE_VOLUME_VFLOOP>iUPPER_MAXSCALE_VOLUME*10)
	{
		m_iMAXSCALE_VOLUME_VFLOOP=iUPPER_MAXSCALE_VOLUME*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_VFLOOP_16,iUPPER_MAXSCALE_VOLUME*10);
	}
	m_iMAXSCALE_FLOW_VFLOOP=getModel()->getI2C()->ReadConfigWord(MAXSCALE_FLOW_VFLOOP_16);
	if(m_iMAXSCALE_FLOW_VFLOOP<G_LOWER_MAXSCALE_FLOW*10 || m_iMAXSCALE_FLOW_VFLOOP>G_UPPER_MAXSCALE_FLOW*10)
	{
		m_iMAXSCALE_FLOW_VFLOOP=G_UPPER_MAXSCALE_FLOW*10;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_VFLOOP_16,G_UPPER_MAXSCALE_FLOW*10);
	}


	m_iMAXSCALE_FOT_PRESSURE=getModel()->getI2C()->ReadConfigWord(MAXSCALE_FOT_PRESSURE_16);
	if(m_iMAXSCALE_FOT_PRESSURE<G_LOWER_MAXSCALE_PRESSURE_FOT || m_iMAXSCALE_FOT_PRESSURE>G_UPPER_MAXSCALE_PRESSURE)
	{
		m_iMAXSCALE_FOT_PRESSURE=G_LOWER_MAXSCALE_PRESSURE_FOT;
		getModel()->getI2C()->WriteConfigWord(MAXSCALE_FOT_PRESSURE_16,m_iMAXSCALE_FOT_PRESSURE);
	}

	//m_iMINSCALE_FOT_XRS=getModel()->getI2C()->ReadConfigWord(MINSCALE_FOT_XRS_16);
	//if(m_iMINSCALE_FOT_XRS> (G_LOWER_MINSCALE_XRS_FOT *(-1)) || m_iMINSCALE_FOT_XRS<G_UPPER_MINSCALE_XRS_FOT)//rku, check FOTGRAPH
	//{
	//	m_iMINSCALE_FOT_XRS=(G_LOWER_MINSCALE_XRS_FOT *(-1));
	//	getModel()->getI2C()->WriteConfigWord(MINSCALE_FOT_XRS_16,m_iMINSCALE_FOT_XRS);
	//}
	m_iMINSCALE_FOT_XRS=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
	m_iMAXSCALE_FOT_XRS=G_UPPER_MAXSCALE_XRS_FOT;
	

	m_eIERelationMode=(eIERelationMode)getModel()->getI2C()->ReadConfigByte(IERELATIONMODE_8);	//->WORKSTATE
	if(m_eIERelationMode<RM_SET_ItoBPM || m_eIERelationMode>RM_SET_ItoE)
	{
		m_eIERelationMode=RM_SET_ItoBPM;
		getModel()->getI2C()->WriteConfigByte(IERELATIONMODE_8,RM_SET_ItoBPM);
	}

	m_eCO2unit=(eCO2unit)getModel()->getI2C()->ReadConfigByte(CO2UNIT_8);
	if(m_eCO2unit<CO2_UNIT_MMHG || m_eCO2unit>CO2_UNIT_PERCENT)
	{
		m_eCO2unit=CO2_UNIT_MMHG;
		getModel()->getI2C()->WriteConfigByte(CO2UNIT_8, CO2_UNIT_MMHG);
	}

	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	
	m_iCO2BaroPressure=getModel()->getI2C()->ReadConfigWord(CO2BAROPRESSURE_16);
	if(m_iCO2BaroPressure<MIN_BAROPRESSURE || m_iCO2BaroPressure>MAX_BAROPRESSURE)
	{
		m_iCO2BaroPressure=760;
		getModel()->getI2C()->WriteConfigWord(CO2BAROPRESSURE_16, m_iCO2BaroPressure);
	}

	m_iCO2TimePumpAutoOn=getModel()->getI2C()->ReadConfigByte(CO2TIMEPUMPAUTOON);
	if(m_iCO2TimePumpAutoOn>30)
	{
		m_iCO2TimePumpAutoOn=10;
		getModel()->getI2C()->WriteConfigByte(CO2TIMEPUMPAUTOON,(BYTE)m_iCO2TimePumpAutoOn);
	}
	
	/*m_iSPO2_SIQlimit=getModel()->getI2C()->ReadConfigByte(SPO2SIQLIMIT_8);
	if(m_iSPO2_SIQlimit<0 || m_iSPO2_SIQlimit>100)
	{
		m_iSPO2_SIQlimit=FACTORY_SPO2_SIQLIMIT;
		getModel()->getI2C()->WriteConfigByte(SPO2SIQLIMIT_8, FACTORY_SPO2_SIQLIMIT);
	}
	else if(m_iSPO2_SIQlimit%5!=0)
	{
		m_iSPO2_SIQlimit=FACTORY_SPO2_SIQLIMIT;
		getModel()->getI2C()->WriteConfigByte(SPO2SIQLIMIT_8, FACTORY_SPO2_SIQLIMIT);
	}*/

	if(getModel()->getI2C()->ReadConfigByte(SPO2FASTSAT_8)==1)
		m_bFastSATon=true;
	else
		m_bFastSATon=false;

	m_eSPO2SensitivityMode=(eSPO2sensitivitymode)getModel()->getI2C()->ReadConfigByte(SPO2SENSITIVITY_8);
	if(m_eSPO2SensitivityMode==SPO2_SENSITIVITY_MAXIMUM)
		m_eSPO2SensitivityMode=SPO2_SENSITIVITY_NORMAL;
	if(m_eSPO2SensitivityMode<SPO2_SENSITIVITY_NORMAL || m_eSPO2SensitivityMode>SPO2_SENSITIVITY_APOD)
	{
		m_eSPO2SensitivityMode=SPO2_SENSITIVITY_NORMAL;
		getModel()->getI2C()->WriteConfigByte(SPO2SENSITIVITY_8, (BYTE)m_eSPO2SensitivityMode);
	}

	m_eSPO2alarmdelay=(eSPO2alarmdelay)getModel()->getI2C()->ReadConfigByte(SPO2ALARMDELAY_8);
	if(m_eSPO2alarmdelay<SPO2_ADELAY_0 || m_eSPO2alarmdelay>SPO2_ADELAY_15)
	{
		m_eSPO2alarmdelay=SPO2_ADELAY_10;
		getModel()->getI2C()->WriteConfigByte(SPO2ALARMDELAY_8,(BYTE)m_eSPO2alarmdelay);
	}

	m_eSPO2averagingTime=(eSPO2averagingtime)getModel()->getI2C()->ReadConfigByte(SPO2AVERAGINGTIME_8);
	if(m_eSPO2averagingTime<SPO2_AVERAGING_2_4 || m_eSPO2averagingTime>SPO2_AVERAGING_16)
	{
		m_eSPO2averagingTime=SPO2_AVERAGING_10;
		getModel()->getI2C()->WriteConfigByte(SPO2AVERAGINGTIME_8, (BYTE)m_eSPO2averagingTime);
	}

	m_iSetupView_SettingBtn=IDC_BTN_SETUP_AUTOOXYCAL;
	m_iSetupView_CO2Btn=IDC_BTN_SETUP_CO2MODULE;
	m_iSetupView_SPO2Btn=IDC_BTN_SETUP_SPO2MODULE;
	m_iALimit_Btn=IDC_BTN_ALARM_MV_HI;
	//m_iFOTViewBtn=IDC_BTN_PARA_FOTAMP;//IDC_BTN_PARA_FOTSTEPS;
	m_iFOTViewBtn=IDC_BTN_PARA_FOT_STEPS;

	m_iParaViewBtn_IPPV=IDC_BTN_PARA_IFLOW;
	m_iParaViewBtn_PSV=IDC_BTN_PARA_IFLOW;
	m_iParaViewBtn_SIMV=IDC_BTN_PARA_IFLOW;
	m_iParaViewBtn_SIMVPSV=IDC_BTN_PARA_IFLOW;
	m_iParaViewBtn_SIPPV=IDC_BTN_PARA_IFLOW;
	m_iParaViewBtn_CPAP=IDC_BTN_PARA_FLOWMIN;
	m_iParaViewBtn_NCPAP=IDC_BTN_PARA_CPAP;
	m_iParaViewBtn_DUOPAP=IDC_BTN_PARA_CPAP;
	m_iParaViewBtn_HFO=IDC_BTN_PARA_HFFLOW;
	m_iParaViewBtn_THERAPIE=IDC_BTN_PARA_THERAPYFLOW;

	CTlsRegistry regLimit(_T("HKCU\\Software\\FabianHFO\\WorkState\\Limits"),true);
	

	WORD iMINRANGE_NEO_ITIME_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_IPPV"), MINRANGE_NEO_ITIME_IPPV);
	WORD iMAXRANGE_PED_ITIME_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_IPPV"), MAXRANGE_PED_ITIME_IPPV);
	//m_iParaDataITime;
	m_iParaDataITime_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_ITIME_IPPV_16);
	if(m_iParaDataITime_IPPV<100 || m_iParaDataITime_IPPV>2000)
	{
		m_iParaDataITime_IPPV=FACTORY_IPPV_ITIME;
		getModel()->getI2C()->WriteConfigWord(PARA_ITIME_IPPV_16,m_iParaDataITime_IPPV);
	}

	WORD iMINRANGE_NEO_ITIME_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_TRIGGER"), MINRANGE_NEO_ITIME_TRIGGER);
	WORD iMAXRANGE_PED_ITIME_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_TRIGGER"), MAXRANGE_PED_ITIME_TRIGGER);
	m_iParaDataITime_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_ITIME_TRIGGER_16);
	if(m_iParaDataITime_TRIGGER<iMINRANGE_NEO_ITIME_TRIGGER || m_iParaDataITime_TRIGGER>iMAXRANGE_PED_ITIME_TRIGGER)
	{
		switch(GetCurMode())//newSPI
		{
		case VM_SIPPV:
			{
				m_iParaDataITime_TRIGGER=FACTORY_SIPPV_ITIME;
			}
			break;
		case VM_SIMV:
			{
				m_iParaDataITime_TRIGGER=FACTORY_SIMV_ITIME;
			}
			break;
		case VM_SIMVPSV:
			{
				m_iParaDataITime_TRIGGER=FACTORY_SIMVPSV_ITIME;
			}
			break;
		case VM_PSV:
			{
				m_iParaDataITime_TRIGGER=FACTORY_PSV_ITIME;
			}
			break;
		default:
			{
				m_iParaDataITime_TRIGGER=FACTORY_IPPV_ITIME;
			}
			break;
		}
		
		getModel()->getI2C()->WriteConfigWord(PARA_ITIME_TRIGGER_16,m_iParaDataITime_TRIGGER);
	}

	WORD iMINRANGE_NEO_ITIME_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_NMODE"), MINRANGE_NEO_ITIME_NMODE);
	WORD iMAXRANGE_PED_ITIME_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_NMODE"), MAXRANGE_PED_ITIME_NMODE);
	//m_iParaDataITime_NMODE;
	m_iParaDataITime_NMODE=getModel()->getI2C()->ReadConfigWord(PARA_ITIME_NMODE_16);
	if(m_iParaDataITime_NMODE<iMINRANGE_NEO_ITIME_NMODE || m_iParaDataITime_NMODE>iMAXRANGE_PED_ITIME_NMODE)
	{
		m_iParaDataITime_NMODE=FACTORY_NMODE_ITIME;
		getModel()->getI2C()->WriteConfigWord(PARA_ITIME_NMODE_16,m_iParaDataITime_NMODE);
	}

	WORD iMINRANGE_NEO_ETIME_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_IPPV"), MINRANGE_NEO_ETIME_IPPV);
	WORD iMAXRANGE_PED_ETIME_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_IPPV"), MAXRANGE_PED_ETIME_IPPV);
	m_iParaDataETIME_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_ETIME_IPPV_16);
	if(m_iParaDataETIME_IPPV<iMINRANGE_NEO_ETIME_IPPV || m_iParaDataETIME_IPPV>iMAXRANGE_PED_ETIME_IPPV)
	{
		m_iParaDataETIME_IPPV=FACTORY_IPPV_ETIME;
		getModel()->getI2C()->WriteConfigWord(PARA_ETIME_IPPV_16,m_iParaDataETIME_IPPV);
	}

	WORD iMINRANGE_NEO_ETIME_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_TRIGGER"), MINRANGE_NEO_ETIME_TRIGGER);
	WORD iMAXRANGE_PED_ETIME_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_TRIGGER"), MAXRANGE_PED_ETIME_TRIGGER);
	//m_iParaDataETime;
	m_iParaDataETIME_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_ETIME_TRIGGER_16);
	if(m_iParaDataETIME_TRIGGER<iMINRANGE_NEO_ETIME_TRIGGER || m_iParaDataETIME_TRIGGER>iMAXRANGE_PED_ETIME_TRIGGER)
	{
		switch(GetCurMode())//newSPI
		{
		case VM_SIPPV:
			{
				m_iParaDataETIME_TRIGGER=FACTORY_SIPPV_ETIME;
			}
			break;
		case VM_SIMV:
			{
				m_iParaDataETIME_TRIGGER=FACTORY_SIMV_ETIME;
			}
			break;
		case VM_SIMVPSV:
			{
				m_iParaDataETIME_TRIGGER=FACTORY_SIMVPSV_ETIME;
			}
			break;
		case VM_PSV:
			{
				m_iParaDataETIME_TRIGGER=FACTORY_PSV_ETIME;
			}
			break;
		default:
			{
				m_iParaDataETIME_TRIGGER=FACTORY_IPPV_ETIME;
			}
			break;
		}
		getModel()->getI2C()->WriteConfigWord(PARA_ETIME_TRIGGER_16,m_iParaDataETIME_TRIGGER);
	}

	WORD iMINRANGE_NEO_ETIME_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_NMODE"), MINRANGE_NEO_ETIME_NMODE);
	WORD iMAXRANGE_PED_ETIME_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_NMODE"), MAXRANGE_PED_ETIME_NMODE);
	//m_iParaDataETime;
	m_iParaDataETIME_NMODE=getModel()->getI2C()->ReadConfigWord(PARA_ETIME_NMODE_16);
	if(m_iParaDataETIME_NMODE<iMINRANGE_NEO_ETIME_NMODE || m_iParaDataETIME_NMODE>iMAXRANGE_PED_ETIME_NMODE)
	{
		switch(GetCurMode())//newSPI
		{
		case VM_NCPAP:
		case VM_DUOPAP:
		case VM_THERAPIE:
			{
				m_iParaDataETIME_NMODE=FACTORY_NMODE_ETIME;
			}
			break;
		default:
			{
				m_iParaDataETIME_NMODE=FACTORY_NMODE_ETIME;
			}
			break;
		}
		getModel()->getI2C()->WriteConfigWord(PARA_ETIME_NMODE_16,m_iParaDataETIME_NMODE);
	}

	WORD iMINRANGE_PED_BPM_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_BPM_IPPV"), MINRANGE_PED_BPM_IPPV);
	WORD iMAXRANGE_NEO_BPM_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BPM_IPPV"), MAXRANGE_NEO_BPM_IPPV);
	m_iParaDataBPM_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_BPM_IPPV_16);
	if(m_iParaDataBPM_IPPV<iMINRANGE_PED_BPM_IPPV || m_iParaDataBPM_IPPV>iMAXRANGE_NEO_BPM_IPPV)
	{
		m_iParaDataBPM_IPPV=FACTORY_IPPV_BPM;
		getModel()->getI2C()->WriteConfigWord(PARA_BPM_IPPV_16,m_iParaDataBPM_IPPV);
	}

	WORD iMINRANGE_PED_BPM_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_BPM_TRIGGER"), MINRANGE_PED_BPM_TRIGGER);
	WORD iMAXRANGE_NEO_BPM_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BPM_TRIGGER"), MAXRANGE_NEO_BPM_TRIGGER);
	//m_iParaDataBPM;
	m_iParaDataBPM_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_BPM_TRIGGER_16);

	if(m_iParaDataBPM_TRIGGER<iMINRANGE_PED_BPM_TRIGGER || m_iParaDataBPM_TRIGGER>iMAXRANGE_NEO_BPM_TRIGGER)
	{
		switch(GetCurMode())//newSPI
		{
		case VM_SIPPV:
			{
				m_iParaDataBPM_TRIGGER=FACTORY_SIPPV_BPM;
			}
			break;
		case VM_SIMV:
			{
				m_iParaDataBPM_TRIGGER=FACTORY_SIMV_BPM;
			}
			break;
		case VM_SIMVPSV:
			{
				m_iParaDataBPM_TRIGGER=FACTORY_SIMVPSV_BPM;
			}
			break;
		case VM_PSV:
			{
				m_iParaDataBPM_TRIGGER=FACTORY_PSV_BPM;
			}
			break;
		default:
			{
				m_iParaDataBPM_TRIGGER=FACTORY_IPPV_BPM;
			}
			break;
		}
		getModel()->getI2C()->WriteConfigWord(PARA_BPM_TRIGGER_16,m_iParaDataBPM_TRIGGER);
	}

	WORD iMINRANGE_NEO_BPM_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_BPM_NMODE"), MINRANGE_NEO_BPM_NMODE);
	WORD iMAXRANGE_PED_BPM_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_BPM_NMODE"), MAXRANGE_PED_BPM_NMODE);
	//m_iParaDataBPM_NMODE;
	m_iParaDataBPM_NMODE=getModel()->getI2C()->ReadConfigWord(PARA_BPM_NMODE_16);
	if(m_iParaDataBPM_NMODE<iMINRANGE_NEO_BPM_NMODE || m_iParaDataBPM_NMODE>iMAXRANGE_PED_BPM_NMODE)
	{
		m_iParaDataBPM_NMODE=FACTORY_NMODE_BPM;
		getModel()->getI2C()->WriteConfigWord(PARA_BPM_NMODE_16,m_iParaDataBPM_NMODE);
	}

	WORD iMINRANGE_NEO_IFLOW_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_IFLOW_IPPV"), MINRANGE_NEO_IFLOW_IPPV);
	WORD iMAXRANGE_PED_IFLOW_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_IFLOW_IPPV"), MAXRANGE_PED_IFLOW_IPPV);
	m_iParaDataIFlow_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_IFLOW_IPPV_16);
	if(m_iParaDataIFlow_IPPV<iMINRANGE_NEO_IFLOW_IPPV || m_iParaDataIFlow_IPPV>iMAXRANGE_PED_IFLOW_IPPV)
	{
		m_iParaDataIFlow_IPPV=FACTORY_IFLOW;
		getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_IPPV_16,m_iParaDataIFlow_IPPV);
	}

	WORD iMINRANGE_NEO_IFLOW_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_IFLOW_TRIGGER"), MINRANGE_NEO_IFLOW_TRIGGER);
	WORD iMAXRANGE_PED_IFLOW_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_IFLOW_TRIGGER"), MAXRANGE_PED_IFLOW_TRIGGER);
	//m_iParaDataIFlow;
	m_iParaDataIFlow_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_IFLOW_TRIGGER_16);
	if(m_iParaDataIFlow_TRIGGER<iMINRANGE_NEO_IFLOW_TRIGGER || m_iParaDataIFlow_TRIGGER>iMAXRANGE_PED_IFLOW_TRIGGER)
	{
		m_iParaDataIFlow_TRIGGER=FACTORY_IFLOW;
		getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_TRIGGER_16,m_iParaDataIFlow_TRIGGER);
	}

	WORD iMINRANGE_NEO_HFFLOW=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFFLOW"), MINRANGE_NEO_HFFLOW);
	WORD iMAXRANGE_PED_HFFLOW=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFFLOW"), MAXRANGE_PED_HFFLOW);
	//m_iParaDataHFFlow;
	m_iParaDataHFFlow=getModel()->getI2C()->ReadConfigWord(PARA_HFFLOW_16);
	if(m_iParaDataHFFlow<iMINRANGE_NEO_HFFLOW || m_iParaDataHFFlow>iMAXRANGE_PED_HFFLOW)
	{
		m_iParaDataHFFlow=FACTORY_HFFLOW;
		getModel()->getI2C()->WriteConfigWord(PARA_HFFLOW_16,m_iParaDataHFFlow);
	}

	WORD iMAXRANGE_PED_RISETIME_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_RISETIME_IPPV"), MAXRANGE_PED_RISETIME_IPPV);
	WORD iMINRANGE_NEO_RISETIME_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_RISETIME_IPPV"), MINRANGE_NEO_RISETIME_IPPV);
	//m_iParaDataPlateau;
	m_iParaDataRisetime_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_RISETIME_IPPV_16);
	//m_iParaDataRisetime_IPPV=0;
	if(m_iParaDataRisetime_IPPV<iMINRANGE_NEO_RISETIME_IPPV || m_iParaDataRisetime_IPPV>iMAXRANGE_PED_RISETIME_IPPV)
	{
		m_iParaDataRisetime_IPPV=FACTORY_RISETIME;
		getModel()->getI2C()->WriteConfigWord(PARA_RISETIME_IPPV_16,m_iParaDataRisetime_IPPV);
	}

	WORD iMAXRANGE_PED_RISETIME_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_RISETIME_TRIGGER"), MAXRANGE_PED_RISETIME_TRIGGER);
	WORD iMINRANGE_NEO_RISETIME_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_RISETIME_TRIGGER"), MINRANGE_NEO_RISETIME_TRIGGER);
	m_iParaDataRisetime_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_RISETIME_TRIGGER_16);
	//m_iParaDataRisetime_TRIGGER=0;
	if(m_iParaDataRisetime_TRIGGER<iMINRANGE_NEO_RISETIME_TRIGGER || m_iParaDataRisetime_TRIGGER>iMAXRANGE_PED_RISETIME_TRIGGER)
	{
		m_iParaDataRisetime_TRIGGER=FACTORY_RISETIME;
		getModel()->getI2C()->WriteConfigWord(PARA_RISETIME_TRIGGER_16,m_iParaDataRisetime_TRIGGER);
	}

	WORD iMINRANGE_NEO_EFLOW_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_EFLOW_IPPV"), MINRANGE_NEO_EFLOW_IPPV);
	WORD iMAXRANGE_PED_EFLOW_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_EFLOW_IPPV"), MAXRANGE_PED_EFLOW_IPPV);
	//m_iParaDataEFlow;
	m_iParaDataEFlow_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_EFLOW_IPPV_16);
	if(m_iParaDataEFlow_IPPV<iMINRANGE_NEO_EFLOW_IPPV || m_iParaDataEFlow_IPPV>iMAXRANGE_PED_EFLOW_IPPV)
	{
		m_iParaDataEFlow_IPPV=FACTORY_EFLOW;
		getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_IPPV_16,m_iParaDataEFlow_IPPV);
	}

	WORD iMINRANGE_NEO_EFLOW_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_EFLOW_TRIGGER"), MINRANGE_NEO_EFLOW_TRIGGER);
	WORD iMAXRANGE_PED_EFLOW_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_EFLOW_TRIGGER"), MAXRANGE_PED_EFLOW_TRIGGER);
	m_iParaDataEFlow_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_EFLOW_TRIGGER_16);
	if(m_iParaDataEFlow_TRIGGER<iMINRANGE_NEO_EFLOW_TRIGGER || m_iParaDataEFlow_TRIGGER>iMAXRANGE_PED_EFLOW_TRIGGER)
	{
		m_iParaDataEFlow_TRIGGER=FACTORY_EFLOW;
		getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_TRIGGER_16,m_iParaDataEFlow_TRIGGER);
	}

	WORD iMINRANGE_PED_THERAPYFLOW=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_THERAPYFLOW"), MINRANGE_PED_THERAPYFLOW);
	WORD iMAXRANGE_PED_THERAPYFLOW=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_THERAPYFLOW"), MAXRANGE_PED_THERAPYFLOW);
	//m_iParaDataTherapieFlow;
	m_iParaDataTherapieFlow=getModel()->getI2C()->ReadConfigWord(PARA_THERAPYFLOW_16);
	if(m_iParaDataTherapieFlow<iMINRANGE_PED_THERAPYFLOW || m_iParaDataTherapieFlow>iMAXRANGE_PED_THERAPYFLOW)
	{
		m_iParaDataTherapieFlow=FACTORY_THERAPYFLOW;
		getModel()->getI2C()->WriteConfigWord(PARA_THERAPYFLOW_16,m_iParaDataTherapieFlow);
	}

	WORD iMINRANGE_PED_FLOWMIN=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FLOWMIN"), MINRANGE_PED_FLOWMIN);
	WORD iMAXRANGE_PED_FLOWMIN=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FLOWMIN"), MAXRANGE_PED_FLOWMIN);
	//m_iParaDataFlowMin;
	m_iParaDataFlowMin=getModel()->getI2C()->ReadConfigWord(PARA_FLOWMIN_16);
	if(m_iParaDataFlowMin<iMINRANGE_PED_FLOWMIN || m_iParaDataFlowMin>iMAXRANGE_PED_FLOWMIN)
	{
		m_iParaDataFlowMin=FACTORY_FLOWMIN;
		getModel()->getI2C()->WriteConfigWord(PARA_FLOWMIN_16,m_iParaDataFlowMin);
	}

	WORD iMINRANGE_PED_PEEP_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PEEP_IPPV"), MINRANGE_PED_PEEP_IPPV);
	WORD iMAXRANGE_PED_PEEP_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PEEP_IPPV"), MAXRANGE_PED_PEEP_IPPV);
	//m_iParaDataPEEP;
	m_iParaDataPEEP_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_PEEP_IPPV_16);
	if(m_iParaDataPEEP_IPPV<iMINRANGE_PED_PEEP_IPPV || m_iParaDataPEEP_IPPV>iMAXRANGE_PED_PEEP_IPPV)
	{
		m_iParaDataPEEP_IPPV=FACTORY_PEEP;
		getModel()->getI2C()->WriteConfigWord(PARA_PEEP_IPPV_16,m_iParaDataPEEP_IPPV);
	}

	WORD iMINRANGE_PED_PEEP_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PEEP_TRIGGER"), MINRANGE_PED_PEEP_TRIGGER);
	WORD iMAXRANGE_PED_PEEP_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PEEP_TRIGGER"), MAXRANGE_PED_PEEP_TRIGGER);
	m_iParaDataPEEP_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_PEEP_TRIGGER_16);
	if(m_iParaDataPEEP_TRIGGER<iMINRANGE_PED_PEEP_TRIGGER || m_iParaDataPEEP_TRIGGER>iMAXRANGE_PED_PEEP_TRIGGER)
	{
		m_iParaDataPEEP_TRIGGER=FACTORY_PEEP;
		getModel()->getI2C()->WriteConfigWord(PARA_PEEP_TRIGGER_16,m_iParaDataPEEP_TRIGGER);
	}

	WORD iMINRANGE_PED_CPAP=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_CPAP"), MINRANGE_PED_CPAP);
	WORD iMAXRANGE_PED_CPAP=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_CPAP"), MAXRANGE_PED_CPAP);
	//m_iParaDataCPAP;
	m_iParaDataCpap=getModel()->getI2C()->ReadConfigWord(PARA_CPAP_16);
	if(m_iParaDataCpap<iMINRANGE_PED_CPAP || m_iParaDataCpap>iMAXRANGE_PED_CPAP)
	{
		m_iParaDataCpap=FACTORY_CPAP;
		getModel()->getI2C()->WriteConfigWord(PARA_CPAP_16,m_iParaDataCpap);
	}

	
	WORD iMINRANGE_PED_CPAP_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_CPAP_NMODE"), MINRANGE_PED_CPAP_NMODE);
	WORD iMAXRANGE_PED_CPAP_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_CPAP_NMODE"), MAXRANGE_PED_CPAP_NMODE);
	//m_iParaDataCPAP_NMODE;
	m_iParaDataNmode=getModel()->getI2C()->ReadConfigWord(PARA_CPAP_NMODE_16);
	if(m_iParaDataNmode<iMINRANGE_PED_CPAP_NMODE || m_iParaDataNmode>iMAXRANGE_PED_CPAP_NMODE)
	{
		m_iParaDataNmode=FACTORY_CPAP;
		getModel()->getI2C()->WriteConfigWord(PARA_CPAP_NMODE_16,50);
	}


	WORD iMINRANGE_PED_PINSP_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PINSP_IPPV"), MINRANGE_PED_PINSP_IPPV);
	WORD iMAXRANGE_PED_PINSP_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PINSP_IPPV"), MAXRANGE_PED_PINSP_IPPV);
	//m_iParaDataPInsp;
	m_iParaDataPInsp_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_PINSP_IPPV_16);
	if(m_iParaDataPInsp_IPPV<iMINRANGE_PED_PINSP_IPPV || m_iParaDataPInsp_IPPV>iMAXRANGE_PED_PINSP_IPPV)
	{
		m_iParaDataPInsp_IPPV=FACTORY_PINSP;
		getModel()->getI2C()->WriteConfigWord(PARA_PINSP_IPPV_16,m_iParaDataPInsp_IPPV);
	}

	WORD iMINRANGE_PED_PINSP_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PINSP_TRIGGER"), MINRANGE_PED_PINSP_TRIGGER);
	WORD iMAXRANGE_PED_PINSP_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PINSP_TRIGGER"), MAXRANGE_PED_PINSP_TRIGGER);
	m_iParaDataPInsp_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_PINSP_TRIGGER_16);
	if(m_iParaDataPInsp_TRIGGER<iMINRANGE_PED_PINSP_TRIGGER || m_iParaDataPInsp_TRIGGER>iMAXRANGE_PED_PINSP_TRIGGER)
	{
		m_iParaDataPInsp_TRIGGER=FACTORY_PINSP;
		getModel()->getI2C()->WriteConfigWord(PARA_PINSP_TRIGGER_16,m_iParaDataPInsp_TRIGGER);
	}

	
	WORD iMINRANGE_PED_PMAXVOLG_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PMAXVOLG_IPPV"), MINRANGE_PED_PMAXVOLG_IPPV);
	WORD iMAXRANGE_PED_PMAXVOLG_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMAXVOLG_IPPV"), MAXRANGE_PED_PMAXVOLG_IPPV);
	//m_iParaDataPInspVolG;
	m_iParaDataPmaxVolG_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_PMAXVOLG_IPPV_16);
	if(m_iParaDataPmaxVolG_IPPV<iMINRANGE_PED_PMAXVOLG_IPPV || m_iParaDataPmaxVolG_IPPV>iMAXRANGE_PED_PMAXVOLG_IPPV)
	{
		m_iParaDataPmaxVolG_IPPV=FACTORY_PMAXVOLG;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_IPPV_16,m_iParaDataPmaxVolG_IPPV);
	}

	WORD iMINRANGE_PED_PMAXVOLG_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PMAXVOLG_TRIGGER"), MINRANGE_PED_PMAXVOLG_TRIGGER);
	WORD iMAXRANGE_PED_PMAXVOLG_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMAXVOLG_TRIGGER"), MAXRANGE_PED_PMAXVOLG_TRIGGER);
	m_iParaDataPmaxVolG_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_PMAXVOLG_TRIGGER_16);
	if(m_iParaDataPmaxVolG_TRIGGER<iMINRANGE_PED_PMAXVOLG_TRIGGER || m_iParaDataPmaxVolG_TRIGGER>iMAXRANGE_PED_PMAXVOLG_TRIGGER)
	{
		m_iParaDataPmaxVolG_TRIGGER=FACTORY_PMAXVOLG;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_TRIGGER_16,m_iParaDataPmaxVolG_TRIGGER);
	}

	WORD iMINRANGE_PED_PPSV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PPSV"), MINRANGE_PED_PPSV);
	WORD iMAXRANGE_PED_PPSV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PPSV"), MAXRANGE_PED_PPSV);
	//m_iParaDataPPSV;
	m_iParaDataPPSV=getModel()->getI2C()->ReadConfigWord(PARA_PPSV_16);
	if(m_iParaDataPPSV<iMINRANGE_PED_PPSV || m_iParaDataPPSV>iMAXRANGE_PED_PPSV)
	{
		m_iParaDataPPSV=FACTORY_PEEP+20;//FACTORY_PPSV;
		getModel()->getI2C()->WriteConfigWord(PARA_PPSV_16,m_iParaDataPPSV);
	}

	//m_iParaDataPPSV=FACTORY_PEEP+20;//FACTORY_PPSV;
	//getModel()->getI2C()->WriteConfigWord(PARA_PPSV_16,m_iParaDataPPSV);

	

	WORD iMINRANGE_PED_PMANUAL=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PMANUAL"), MINRANGE_PED_PMANUAL);
	WORD iMAXRANGE_PED_PMANUAL=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMANUAL"), MAXRANGE_PED_PMANUAL);
	//m_iParaDataPManual;
	m_iParaDataPManual_HFO=getModel()->getI2C()->ReadConfigWord(PARA_PMAN_HFO_16);
	if(m_iParaDataPManual_HFO<iMINRANGE_PED_PMANUAL || m_iParaDataPManual_HFO>iMAXRANGE_PED_PMANUAL)
	{
		m_iParaDataPManual_HFO=FACTORY_PMAN;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAN_HFO_16,m_iParaDataPManual_HFO);
	}

	m_iParaDataPManual_CPAP=getModel()->getI2C()->ReadConfigWord(PARA_PMAN_CPAP_16);
	if(m_iParaDataPManual_CPAP<iMINRANGE_PED_PMANUAL || m_iParaDataPManual_CPAP>iMAXRANGE_PED_PMANUAL)
	{
		m_iParaDataPManual_CPAP=FACTORY_PMAN;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAN_CPAP_16,m_iParaDataPManual_CPAP);
	}

	WORD iMINRANGE_PED_PMANUAL_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PMANUAL_NMODE"), MINRANGE_PED_PMANUAL_NMODE);
	WORD iMAXRANGE_PED_PMANUAL_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMANUAL_NMODE"), MAXRANGE_PED_PMANUAL_NMODE);
	//m_iParaDataPManual_NMODE;
	m_iParaDataPManual_NMODE=getModel()->getI2C()->ReadConfigWord(PARA_PMAN_NMODE_16);
	if(m_iParaDataPManual_NMODE<iMINRANGE_PED_PMANUAL_NMODE || m_iParaDataPManual_NMODE>iMAXRANGE_PED_PMANUAL_NMODE)
	{
		m_iParaDataPManual_NMODE=FACTORY_PMAN;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAN_NMODE_16,m_iParaDataPManual_NMODE);
	}

	WORD iMINRANGE_PED_O2=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_O2"), MINRANGE_PED_O2);
	WORD iMAXRANGE_PED_O2=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_O2"), MAXRANGE_PED_O2);
	//m_iParaDataO2;
	m_iParaDataO2=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_O2_16);
	if(m_iParaDataO2<iMINRANGE_PED_O2 || m_iParaDataO2>iMAXRANGE_PED_O2)
	{
		m_iParaDataO2=FACTORY_O2;
		getModel()->getI2C()->WriteConfigWord(PARA_O2_16,m_iParaDataO2);
	}

	WORD iMINRANGE_PED_O2FLUSH=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_O2FLUSH"), MINRANGE_PED_O2FLUSH);
	WORD iMAXRANGE_PED_O2FLUSH=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_O2FLUSH"), MAXRANGE_PED_O2FLUSH);
	//m_iParaDataO2Flush;
	m_iParaDataO2Flush=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_O2FLUSH_16);
	if(m_iParaDataO2Flush<iMINRANGE_PED_O2FLUSH || m_iParaDataO2Flush>iMAXRANGE_PED_O2FLUSH)
	{
		m_iParaDataO2Flush=FACTORY_O2FLUSH;
		getModel()->getI2C()->WriteConfigWord(PARA_O2FLUSH_16,m_iParaDataO2Flush);
	}

	//m_iO2Difference;
	m_iO2Difference=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_O2DIFF_16);
	if(m_iO2Difference<MAXDIFF_O2_FLUSH || m_iO2Difference>79)
	{
		m_iO2Difference=MAXDIFF_O2_FLUSH;
		getModel()->getI2C()->WriteConfigWord(PARA_O2DIFF_16,m_iO2Difference);
	}

	if(m_iParaDataO2>m_iParaDataO2Flush-MAXDIFF_O2_FLUSH)
	{
		m_iParaDataO2=FACTORY_O2;
		getModel()->getI2C()->WriteConfigWord(PARA_O2_16,m_iParaDataO2);
		m_iParaDataO2Flush=FACTORY_O2FLUSH;
		getModel()->getI2C()->WriteConfigWord(PARA_O2FLUSH_16,m_iParaDataO2Flush);
		m_iO2Difference=MAXDIFF_O2_FLUSH;
		getModel()->getI2C()->WriteConfigWord(PARA_O2DIFF_16,m_iO2Difference);
	}

	//PRICO02 dependency of range regarding parameter of FiO2
	m_iPRICO_FIO2lowRange=getModel()->getI2C()->ReadConfigByte(PRICOFIO2LOW_8);
	m_iPRICO_FIO2highRange=getModel()->getI2C()->ReadConfigByte(PRICOFIO2HIGH_8);
	if(m_iPRICO_FIO2highRange>100 || m_iPRICO_FIO2highRange<22)
	{
		getModel()->getI2C()->WriteConfigByte(PRICOFIO2HIGH_8,100);
		m_iPRICO_FIO2highRange=100;
	}
	/*else if(m_iPRICO_FIO2highRange<m_iParaDataO2)
	{
		getModel()->getI2C()->WriteConfigByte(PRICOFIO2HIGH_8,m_iParaDataO2);
		m_iPRICO_FIO2highRange=m_iParaDataO2;
	}*/
	if(m_iPRICO_FIO2lowRange>100 || m_iPRICO_FIO2lowRange>=m_iPRICO_FIO2highRange)
	{
		m_iPRICO_FIO2lowRange=21;
		getModel()->getI2C()->WriteConfigByte(PRICOFIO2LOW_8,m_iPRICO_FIO2lowRange);
	}
	/*else if(m_iPRICO_FIO2lowRange>m_iParaDataO2)
	{
		m_iPRICO_FIO2lowRange=m_iParaDataO2;
		getModel()->getI2C()->WriteConfigByte(PRICOFIO2LOW_8,m_iParaDataO2);
	}*/


	m_iPRICO_SPO2lowRange=getModel()->getI2C()->ReadConfigByte(PRICOSPO2LOW_8);
	m_iPRICO_SPO2highRange=getModel()->getI2C()->ReadConfigByte(PRICOSPO2HIGH_8);
	if(m_iPRICO_SPO2highRange>100 || m_iPRICO_SPO2highRange==0)
	{
		getModel()->getI2C()->WriteConfigByte(PRICOSPO2HIGH_8,FACTORY_ALIMIT_VAL_SPO2MAX);
		m_iPRICO_SPO2highRange=FACTORY_ALIMIT_VAL_SPO2MAX/10;
	}
	if(m_iPRICO_SPO2lowRange>100 || m_iPRICO_SPO2lowRange>=m_iPRICO_SPO2highRange)
	{
		m_iPRICO_SPO2lowRange=m_iPRICO_SPO2highRange-1;
		getModel()->getI2C()->WriteConfigByte(PRICOSPO2HIGH_8,m_iPRICO_SPO2lowRange);
	}

	

	//m_iPPSVDifference;
	m_iPPSVDifference=getModel()->getI2C()->ReadConfigWord(PARA_PPSVDIFF_16);
	if(m_iPPSVDifference<DIFF_PEEP_PINSP /*|| m_iPPSVDifference>79*/)
	{
		m_iPPSVDifference=DIFF_PEEP_PINSP;
		getModel()->getI2C()->WriteConfigWord(PARA_PPSVDIFF_16,m_iPPSVDifference);
	}

	/*m_iPPSVDifference=DIFF_PEEP_PINSP;
	getModel()->getI2C()->WriteConfigWord(PARA_PPSVDIFF_16,m_iPPSVDifference);*/

	//m_iPmeanDifference;
	m_iPmeanDifference=getModel()->getI2C()->ReadConfigWord(PARA_PMEANDIFF_16);
	if(m_iPmeanDifference<MAXDIFF_PMEANREC /*|| m_iPmeanDifference>79*/)
	{
		m_iPmeanDifference=MAXDIFF_PMEANREC;
		getModel()->getI2C()->WriteConfigWord(PARA_PMEANDIFF_16,m_iPmeanDifference);
	}


	//m_iParaDataVLimit;
	WORD iMINRANGE_NEO_VLIMIT_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VLIMIT_TRIGGER"), MINRANGE_NEO_VLIMIT_TRIGGER);
	WORD iMAXRANGE_PED_VLIMIT_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VLIMIT_TRIGGER"), MAXRANGE_PED_VLIMIT_TRIGGER);
	m_iParaDataVLimit_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_VLIMIT_TRIGGER_16);
	if(m_iParaDataVLimit_TRIGGER<iMINRANGE_NEO_VLIMIT_TRIGGER || m_iParaDataVLimit_TRIGGER>iMAXRANGE_PED_VLIMIT_TRIGGER)
	{
		m_iParaDataVLimit_TRIGGER=FACTORY_VLIMIT;
		getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_TRIGGER_16,m_iParaDataVLimit_TRIGGER);
	}

	WORD iMINRANGE_NEO_VLIMIT_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VLIMIT_IPPV"), MINRANGE_NEO_VLIMIT_IPPV);
	WORD iMAXRANGE_PED_VLIMIT_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VLIMIT_IPPV"), MAXRANGE_PED_VLIMIT_IPPV);
	m_iParaDataVLimit_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_VLIMIT_IPPV_16);
	if(m_iParaDataVLimit_IPPV<iMINRANGE_NEO_VLIMIT_IPPV || m_iParaDataVLimit_IPPV>iMAXRANGE_PED_VLIMIT_IPPV)
	{
		m_iParaDataVLimit_IPPV=FACTORY_VLIMIT;
		getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_IPPV_16,m_iParaDataVLimit_IPPV);
	}

	
	//m_bParaDataVLimitOn;
	if(getModel()->getI2C()->ReadConfigByte(PARA_VLIMITSTATE_TRIGGER_8)==1)
	{
		if(getModel()->getI2C()->ReadConfigByte(PARA_VGARANTSTATE_TRIGGER_8)==1)
		{
			getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8,0);
			m_bParaDataVLimitOn_TRIGGER=false;
		}
		else
		{
			m_bParaDataVLimitOn_TRIGGER=true;
		}
	}
	else
	{
		m_bParaDataVLimitOn_TRIGGER=false;
	}

	if(getModel()->getI2C()->ReadConfigByte(PARA_VLIMITSTATE_IPPV_8)==1)
	{
		if(getModel()->getI2C()->ReadConfigByte(PARA_VGARANTSTATE_IPPV_8)==1)
		{
			getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_IPPV_8,0);
			m_bParaDataVLimitOn_IPPV=false;
		}
		else
		{
			m_bParaDataVLimitOn_IPPV=true;
		}
	}
	else
	{
		m_bParaDataVLimitOn_IPPV=false;
	}

	WORD iMINRANGE_NEO_VGARANT_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VGARANT_TRIGGER"), MINRANGE_NEO_VGARANT_TRIGGER);
	WORD iMAXRANGE_PED_VGARANT_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VGARANT_TRIGGER"), MAXRANGE_PED_VGARANT_TRIGGER);
	//m_iParaDataVGarant;
	m_iParaDataVGarant_TRIGGER=getModel()->getI2C()->ReadConfigWord(PARA_VGARANT_TRIGGER_16);
	if(m_iParaDataVGarant_TRIGGER<iMINRANGE_NEO_VGARANT_TRIGGER || m_iParaDataVGarant_TRIGGER>iMAXRANGE_PED_VGARANT_TRIGGER)
	{
		m_iParaDataVGarant_TRIGGER=FACTORY_VGARANT;
		getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_TRIGGER_16,m_iParaDataVGarant_TRIGGER);
	}

	WORD iMINRANGE_NEO_VGARANT_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VGARANT_IPPV"), MINRANGE_NEO_VGARANT_IPPV);
	WORD iMAXRANGE_PED_VGARANT_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VGARANT_IPPV"), MAXRANGE_PED_VGARANT_IPPV);
	//m_iParaDataVGarant;
	m_iParaDataVGarant_IPPV=getModel()->getI2C()->ReadConfigWord(PARA_VGARANT_IPPV_16);
	if(m_iParaDataVGarant_IPPV<iMINRANGE_NEO_VGARANT_IPPV || m_iParaDataVGarant_IPPV>iMAXRANGE_PED_VGARANT_IPPV)
	{
		m_iParaDataVGarant_IPPV=FACTORY_VGARANT;
		getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_IPPV_16,m_iParaDataVGarant_IPPV);
	}

	//m_bParaDataVGarantOn;
	if(getModel()->getI2C()->ReadConfigByte(PARA_VGARANTSTATE_TRIGGER_8)==1)
		m_bParaDataVGarantOn_TRIGGER=true;
	else
		m_bParaDataVGarantOn_TRIGGER=false;

	if(getModel()->getI2C()->ReadConfigByte(PARA_VGARANTSTATE_IPPV_8)==1)
		m_bParaDataVGarantOn_IPPV=true;
	else
		m_bParaDataVGarantOn_IPPV=false;

	WORD iMAXRANGE_PED_TRIGGER_CONV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_CONV"), MAXRANGE_PED_TRIGGER_CONV);
	WORD iMINRANGE_PED_TRIGGER_CONV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_CONV"), MINRANGE_PED_TRIGGER_CONV);
	//m_iParaDataTrigger;
	m_iParaDataTrigger_CONV=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_TRIGGER_CONVENT_16);
	if(m_iParaDataTrigger_CONV<iMINRANGE_PED_TRIGGER_CONV || m_iParaDataTrigger_CONV>iMAXRANGE_PED_TRIGGER_CONV)
	{
		switch(GetCurMode())//newSPI
		{
		case VM_SIPPV:
			{
				m_iParaDataTrigger_CONV=FACTORY_SIPPV_TRIGGER;
			}
			break;
		case VM_SIMV:
			{
				m_iParaDataTrigger_CONV=FACTORY_SIMV_TRIGGER;
			}
			break;
		case VM_SIMVPSV:
			{
				m_iParaDataTrigger_CONV=FACTORY_SIMVPSV_TRIGGER;
			}
			break;
		case VM_PSV:
			{
				m_iParaDataTrigger_CONV=FACTORY_PSV_TRIGGER;
			}
			break;
		default:
			{
				m_iParaDataTrigger_CONV=FACTORY_SIPPV_BPM;
			}
			break;
		}
		getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CONVENT_16,m_iParaDataTrigger_CONV);
	}

	

	WORD iMAXRANGE_PED_TRIGGER_CPAP=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_CPAP"), MAXRANGE_PED_TRIGGER_CPAP);
	WORD iMINRANGE_PED_TRIGGER_CPAP=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_CPAP"), MINRANGE_PED_TRIGGER_CPAP);
	m_iParaDataTrigger_CPAP=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_TRIGGER_CPAP_16);
	if(m_iParaDataTrigger_CPAP<iMINRANGE_PED_TRIGGER_CPAP || m_iParaDataTrigger_CPAP>iMAXRANGE_PED_TRIGGER_CPAP)
	{
		m_iParaDataTrigger_CPAP=FACTORY_CPAP_TRIGGER;
		getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CPAP_16,m_iParaDataTrigger_CPAP);
	}

	WORD iMAXRANGE_PED_TRIGGER_DUOPAP=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_DUOPAP"), MAXRANGE_TRIGGER_OFF);
	WORD iMINRANGE_PED_TRIGGER_DUOPAP=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_DUOPAP"), MINRANGE_PED_TRIGGER_DUOPAP);
	m_iParaDataTrigger_DUOPAP=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_TRIGGER_DUOPAP_16);
	if(m_iParaDataTrigger_DUOPAP<iMINRANGE_PED_TRIGGER_DUOPAP || m_iParaDataTrigger_DUOPAP>iMAXRANGE_PED_TRIGGER_DUOPAP)
	{
		m_iParaDataTrigger_DUOPAP=FACTORY_DUOPAP_TRIGGER;
		getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_DUOPAP_16,m_iParaDataTrigger_DUOPAP);
	}

	WORD iMAXRANGE_PED_TRIGGER_NCPAP=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_NCPAP"), MAXRANGE_TRIGGER_OFF);
	WORD iMINRANGE_PED_TRIGGER_NCPAP=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_NCPAP"), MINRANGE_PED_TRIGGER_NCPAP);
	m_iParaDataTrigger_NCPAP=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_TRIGGER_NCPAP_16);
	if(m_iParaDataTrigger_NCPAP<iMINRANGE_PED_TRIGGER_NCPAP || m_iParaDataTrigger_NCPAP>iMAXRANGE_PED_TRIGGER_NCPAP)
	{
		m_iParaDataTrigger_NCPAP=FACTORY_NCPAP_TRIGGER;
		getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_NCPAP_16,m_iParaDataTrigger_NCPAP);
	}

	WORD iMAXRANGE_PED_BACKUP=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_BACKUP"), MAXRANGE_PED_BACKUP);
	//m_iParaDataBackup;
	m_iParaDataBackup=(BYTE)getModel()->getI2C()->ReadConfigWord(PARA_BACKUP_16);
	if(m_iParaDataBackup>iMAXRANGE_PED_BACKUP)
	{
		m_iParaDataBackup=FACTORY_CPAP_BACKUP;
		getModel()->getI2C()->WriteConfigWord(PARA_BACKUP_16,0);
	}

	//m_iParaDataHFAmpl;
	WORD iMINRANGE_NEO_HFAMPL=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFAMPL"), MINRANGE_NEO_HFAMPL);
	WORD iMAXRANGE_NEO_HFAMPL=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFAMPL"), MAXRANGE_NEO_HFAMPL);

	m_iParaDataHFAmpl=getModel()->getI2C()->ReadConfigWord(PARA_HFO_HFAMPL_16);
	if(m_iParaDataHFAmpl<iMINRANGE_NEO_HFAMPL || m_iParaDataHFAmpl>iMAXRANGE_NEO_HFAMPL)
	{
		m_iParaDataHFAmpl=FACTORY_HFO_HFAMPL;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFAMPL_16,m_iParaDataHFAmpl);
	}


	WORD iMINRANGE_NEO_HFAMPLVOLG=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFAMPLVOLG"), MINRANGE_NEO_HFAMPLVOLG);
	WORD iMAXRANGE_NEO_HFAMPLVOLG=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFAMPLVOLG_CONICSPRING"), MAXRANGE_NEO_HFAMPLVOLG);

	//m_iParaDataHFAmplmax;
	m_iParaDataHFAmplmax=getModel()->getI2C()->ReadConfigWord(PARA_HFO_HFAMPL_VOLG_16);
	if(m_iParaDataHFAmplmax<iMINRANGE_NEO_HFAMPLVOLG || m_iParaDataHFAmplmax>iMAXRANGE_NEO_HFAMPLVOLG)
	{
		m_iParaDataHFAmplmax=FACTORY_HFO_HFAMPL;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFAMPL_VOLG_16,m_iParaDataHFAmplmax);
	}
	

	WORD iMINRANGE_PED_HFFREQ=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_HFFREQ"), MINRANGE_PED_HFFREQ);
	WORD iMAXRANGE_PED_HFFREQ=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFFREQ"), MAXRANGE_PED_HFFREQ);
	//m_iParaDataHFFreq;
	m_iParaDataHFFreq=getModel()->getI2C()->ReadConfigWord(PARA_HFO_HFFREQ_16);
	if(m_iParaDataHFFreq<iMINRANGE_PED_HFFREQ || m_iParaDataHFFreq>iMAXRANGE_PED_HFFREQ)
	{
		m_iParaDataHFFreq=FACTORY_HFO_HFFREQ;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFFREQ_16,m_iParaDataHFFreq);
	}

	WORD iMINRANGE_PED_ITIME_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ITIME_REC"), MINRANGE_PED_ITIME_REC);
	WORD iMAXRANGE_PED_ITIME_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_REC"), MAXRANGE_PED_ITIME_REC);
	m_iParaDataHFITimeRec=getModel()->getI2C()->ReadConfigWord(PARA_HFO_ITIMEREC16);
	if(m_iParaDataHFITimeRec<iMINRANGE_PED_ITIME_REC || m_iParaDataHFITimeRec>iMAXRANGE_PED_ITIME_REC)
	{
		m_iParaDataHFITimeRec=FACTORY_HFO_ITIMEREC;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_ITIMEREC16,m_iParaDataHFITimeRec);
	}

	WORD iMINRANGE_PED_FREQ_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FREQ_REC"), MINRANGE_PED_FREQ_REC);
	WORD iMAXRANGE_PED_FREQ_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FREQ_REC"), MAXRANGE_PED_FREQ_REC);
	m_iParaDataHFFREQ_REC=getModel()->getI2C()->ReadConfigWord(PARA_HFO_FREQREC16);
	/*if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==false && m_iParaDataHFFREQ_REC!=0)
	{
		m_iParaDataHFFREQ_REC=FACTORY_HFO_FREQREC;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_FREQREC16,m_iParaDataHFFREQ_REC);
	}
	else */if(m_iParaDataHFFREQ_REC<iMINRANGE_PED_FREQ_REC || m_iParaDataHFFREQ_REC>iMAXRANGE_PED_FREQ_REC)
	{
		m_iParaDataHFFREQ_REC=FACTORY_HFO_FREQREC;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_FREQREC16,m_iParaDataHFFREQ_REC);
	}


	WORD iMINRANGE_PED_PMEAN_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PMEAN_REC"), MINRANGE_PED_PMEAN_REC);
	WORD iMAXRANGE_PED_PMEAN_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMEAN_REC"), MAXRANGE_PED_PMEAN_REC);

	m_iParaDataHFPMeanRec=getModel()->getI2C()->ReadConfigWord(PARA_HFO_PMEANREC16);
	if(m_iParaDataHFPMeanRec<iMINRANGE_PED_PMEAN_REC || m_iParaDataHFPMeanRec>iMAXRANGE_PED_PMEAN_REC)
	{
		m_iParaDataHFPMeanRec=FACTORY_HFO_PMEANREC;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_PMEANREC16,m_iParaDataHFPMeanRec);
	}

	WORD iMAXRANGE_NEO_HFPMEAN=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFPMEAN"), MAXRANGE_NEO_HFPMEAN);

	//m_iParaDataHFPmean;
	m_iParaDataHFPmean=getModel()->getI2C()->ReadConfigWord(PARA_HFO_PMEAN_16);
	if(m_iParaDataHFPmean>iMAXRANGE_NEO_HFPMEAN)
	{
		m_iParaDataHFPmean=FACTORY_HFO_PMEAN;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_PMEAN_16,m_iParaDataHFPmean);
	}
	
	WORD iMINRANGE_NEO_HFVGARANT=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFVGARANT"), MINRANGE_NEO_HFVGARANT);
	WORD iMAXRANGE_PED_HFVGARANT=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFVGARANT"), MAXRANGE_PED_HFVGARANT);
	//m_iParaDataHFVGarant;
	m_iParaDataHFVGarant=getModel()->getI2C()->ReadConfigWord(PARA_HFO_VGARANT_16);
	if(m_iParaDataHFVGarant<iMINRANGE_NEO_HFVGARANT ||m_iParaDataHFVGarant>iMAXRANGE_PED_HFVGARANT)
	{
		m_iParaDataHFVGarant=FACTORY_HFO_VGARANT;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_VGARANT_16,m_iParaDataHFVGarant);
	}
	
	//m_bParaDataHFVGarantState;
	if(getModel()->getI2C()->ReadConfigByte(PARA_HFO_VGARANTSTATE_8)==1)
		m_bParaDataHFVGarantState=true;
	else
		m_bParaDataHFVGarantState=false;

	//m_iParaDataHFIERatio;
	m_iParaDataHFIERatio=(eRatioIE)getModel()->getI2C()->ReadConfigByte(PARA_HFO_IERATIO_8);
	if(m_iParaDataHFIERatio<RIE_1_3 || m_iParaDataHFIERatio>RIE_1_1)
	{
		m_iParaDataHFIERatio=(eRatioIE)FACTORY_HFO_IERATIO;
		getModel()->getI2C()->WriteConfigByte(PARA_HFO_IERATIO_8,m_iParaDataHFIERatio);
	}


	m_iAlarmlimitSPO2max=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2MAX_16);
	//getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_16,1000);
	
	if(m_iAlarmlimitSPO2max==0)
		m_iAlarmlimitSPO2max=FACTORY_ALIMIT_VAL_SPO2MAX;
	else
		m_iAlarmlimitSPO2max=(m_iAlarmlimitSPO2max/10)*10;
	if(m_iAlarmlimitSPO2max<20 || m_iAlarmlimitSPO2max>1000)
	{
		m_iAlarmlimitSPO2max=FACTORY_ALIMIT_VAL_SPO2MAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_16,m_iAlarmlimitSPO2max);
	}

	m_iAlarmlimitStateSPO2max=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2MAX_8);
	if(m_iAlarmlimitStateSPO2max<AL_ON || m_iAlarmlimitStateSPO2max>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2max=FACTORY_ALIMIT_STATE_SPO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MAX_8,m_iAlarmlimitStateSPO2max);
	}


	m_iAlarmlimitSPO2min=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2MIN_16);
	//getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_16,10);
	if(m_iAlarmlimitSPO2min!=0)
		m_iAlarmlimitSPO2min=(m_iAlarmlimitSPO2min/10)*10;
	if(m_iAlarmlimitSPO2min<0 || m_iAlarmlimitSPO2min>980)
	{
		m_iAlarmlimitSPO2min=FACTORY_ALIMIT_VAL_SPO2MIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_16,m_iAlarmlimitSPO2min);
	}

	m_iAlarmlimitStateSPO2min=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2MIN_8);
	if(m_iAlarmlimitStateSPO2min<AL_ON || m_iAlarmlimitStateSPO2min>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2min=FACTORY_ALIMIT_STATE_SPO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MIN_8,m_iAlarmlimitStateSPO2min);
	}

	//check SpO2 value against limits
	if(m_iAlarmlimitSPO2max<m_iAlarmlimitSPO2min+20)
	{
		//set factory
		m_iAlarmlimitSPO2max=FACTORY_ALIMIT_VAL_SPO2MAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_16,m_iAlarmlimitSPO2max);

		m_iAlarmlimitSPO2min=FACTORY_ALIMIT_VAL_SPO2MIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_16,m_iAlarmlimitSPO2min);
	}



	m_iAlarmlimitPulseRatemax=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PULSERATEMAX_16);
	if(m_iAlarmlimitPulseRatemax<0 || m_iAlarmlimitPulseRatemax>240)
	{
		m_iAlarmlimitPulseRatemax=FACTORY_ALIMIT_VAL_PULSERATEMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_16,m_iAlarmlimitPulseRatemax);
	}

	m_iAlarmlimitStatePulseRatemax=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PULSERATEMAX_8);
	if(m_iAlarmlimitStatePulseRatemax<AL_ON || m_iAlarmlimitStatePulseRatemax>AL_AUTO)
	{
		m_iAlarmlimitStatePulseRatemax=FACTORY_ALIMIT_STATE_PULSERATEMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMAX_8,m_iAlarmlimitStatePulseRatemax);
	}
	
	m_iAlarmlimitPulseRatemin=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PULSERATEMIN_16);
	if(m_iAlarmlimitPulseRatemin<0 || m_iAlarmlimitPulseRatemin>240)
	{
		m_iAlarmlimitPulseRatemin=FACTORY_ALIMIT_VAL_PULSERATEMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_16,m_iAlarmlimitPulseRatemin);
	}

	m_iAlarmlimitStatePulseRatemin=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PULSERATEMIN_8);
	if(m_iAlarmlimitStatePulseRatemin<AL_ON || m_iAlarmlimitStatePulseRatemin>AL_AUTO)
	{
		m_iAlarmlimitStatePulseRatemin=FACTORY_ALIMIT_STATE_PULSERATEMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMIN_8,m_iAlarmlimitStatePulseRatemin);
	}

	//check pulse rate value against limits
	if(m_iAlarmlimitPulseRatemax<m_iAlarmlimitPulseRatemin+2)
	{
		//set factory
		m_iAlarmlimitPulseRatemax=FACTORY_ALIMIT_VAL_PULSERATEMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_16,m_iAlarmlimitPulseRatemax);

		m_iAlarmlimitPulseRatemin=FACTORY_ALIMIT_VAL_PULSERATEMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_16,m_iAlarmlimitPulseRatemin);
	}

	m_iAlarmlimitSPO2_PImin=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2_PIMIN_16);
	if(m_iAlarmlimitSPO2_PImin<0 || m_iAlarmlimitSPO2_PImin>20000)
	{
		m_iAlarmlimitSPO2_PImin=FACTORY_ALIMIT_VAL_SPO2_PIMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_16,m_iAlarmlimitSPO2_PImin);
	}

	m_iAlarmlimitStateSPO2_PImin=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2_PIMIN_8);
	if(m_iAlarmlimitStateSPO2_PImin<AL_ON || m_iAlarmlimitStateSPO2_PImin>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2_PImin=FACTORY_ALIMIT_STATE_SPO2_PIMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_PIMIN_8,m_iAlarmlimitStateSPO2_PImin);
	}





	m_iAlarmlimitSPO2_SIQmin=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2_SIQMIN_16);
	if(m_iAlarmlimitSPO2_SIQmin<0 || m_iAlarmlimitSPO2_SIQmin>100)
	{
		m_iAlarmlimitSPO2_SIQmin=FACTORY_ALIMIT_VAL_SPO2_SIQMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_SIQMIN_16,m_iAlarmlimitSPO2_SIQmin);
	}

	m_iAlarmlimitStateSPO2_SIQmin=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2_SIQMIN_8);
	if(m_iAlarmlimitStateSPO2_SIQmin<AL_ON || m_iAlarmlimitStateSPO2_SIQmin>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2_SIQmin=FACTORY_ALIMIT_STATE_SPO2_SIQMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_SIQMIN_8,m_iAlarmlimitStateSPO2_SIQmin);
	}



	m_iAlarmlimitETCO2max=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MAX_16);
	m_iAlarmlimitETCO2max=(int)(CTlsFloat::Round(((double)m_iAlarmlimitETCO2max)/10, 0))*10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			//m_iAlarmlimitETCO2max=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MAX_KPA_16);
			if(m_iAlarmlimitETCO2max<MINRANGE_HF_NEONATAL_ETCO2MIN_KPA || m_iAlarmlimitETCO2max>MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA)
			{
				m_iAlarmlimitETCO2max=FACTORY_ALIMIT_VAL_ETCO2MAX_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_16,m_iAlarmlimitETCO2max);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			//m_iAlarmlimitETCO2max=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MAX_VOL_16);
			if(m_iAlarmlimitETCO2max<MINRANGE_HF_NEONATAL_ETCO2MIN_VOL || m_iAlarmlimitETCO2max>MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL)
			{
				m_iAlarmlimitETCO2max=FACTORY_ALIMIT_VAL_ETCO2MAX_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_16,m_iAlarmlimitETCO2max);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			//m_iAlarmlimitETCO2max=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MAX_MMHG_16);
			if(m_iAlarmlimitETCO2max<MINRANGE_HF_NEONATAL_ETCO2MIN_MMHG || m_iAlarmlimitETCO2max>MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG)
			{
				m_iAlarmlimitETCO2max=FACTORY_ALIMIT_VAL_ETCO2MAX_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_16,m_iAlarmlimitETCO2max);
			}
		}
		break;
	}
	

	m_iAlarmlimitStateETCO2max=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_ETCO2MAX_8);
	if(m_iAlarmlimitStateETCO2max<AL_ON || m_iAlarmlimitStateETCO2max>AL_AUTO)
	{
		m_iAlarmlimitStateETCO2max=FACTORY_ALIMIT_STATE_ETCO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MAX_8,m_iAlarmlimitStateETCO2max);
	}

	m_iAlarmlimitETCO2min=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MIN_16);
	m_iAlarmlimitETCO2min=(int)(CTlsFloat::Round(((double)m_iAlarmlimitETCO2min)/10, 0))*10;

	if(m_iAlarmlimitETCO2min>=m_iAlarmlimitETCO2max)
		m_iAlarmlimitETCO2min=m_iAlarmlimitETCO2max-10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitETCO2min<MINRANGE_HF_NEONATAL_ETCO2MIN_KPA || m_iAlarmlimitETCO2min>MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA)
			{
				m_iAlarmlimitETCO2min=FACTORY_ALIMIT_VAL_ETCO2MIN_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_16,m_iAlarmlimitETCO2min);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitETCO2min<MINRANGE_HF_NEONATAL_ETCO2MIN_VOL || m_iAlarmlimitETCO2min>MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL)
			{
				m_iAlarmlimitETCO2min=FACTORY_ALIMIT_VAL_ETCO2MIN_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_16,m_iAlarmlimitETCO2min);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitETCO2min<MINRANGE_HF_NEONATAL_ETCO2MIN_MMHG || m_iAlarmlimitETCO2min>MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG)
			{
				m_iAlarmlimitETCO2min=FACTORY_ALIMIT_VAL_ETCO2MIN_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_16,m_iAlarmlimitETCO2min);
			}
		}
		break;
	}
	

	m_iAlarmlimitStateETCO2min=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_ETCO2MIN_8);
	if(m_iAlarmlimitStateETCO2min<AL_ON || m_iAlarmlimitStateETCO2min>AL_AUTO)
	{
		m_iAlarmlimitStateETCO2min=FACTORY_ALIMIT_STATE_ETCO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MIN_8,m_iAlarmlimitStateETCO2min);
	}

	//###################################################
	m_iAlarmlimitFICO2max=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_FICO2MAX_16);
	m_iAlarmlimitFICO2max=(int)(CTlsFloat::Round(((double)m_iAlarmlimitFICO2max)/10, 0))*10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitFICO2max<MINRANGE_HF_NEONATAL_FICO2MIN_KPA || m_iAlarmlimitFICO2max>MAXRANGE_HF_NEONATAL_FICO2MAX_KPA)
			{
				m_iAlarmlimitFICO2max=FACTORY_ALIMIT_VAL_FICO2MAX_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_16,m_iAlarmlimitFICO2max);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitFICO2max<MINRANGE_HF_NEONATAL_FICO2MIN_VOL || m_iAlarmlimitFICO2max>MAXRANGE_HF_NEONATAL_FICO2MAX_VOL)
			{
				m_iAlarmlimitFICO2max=FACTORY_ALIMIT_VAL_FICO2MAX_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_16,m_iAlarmlimitFICO2max);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitFICO2max<MINRANGE_HF_NEONATAL_FICO2MIN_MMHG || m_iAlarmlimitFICO2max>MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG)
			{
				m_iAlarmlimitFICO2max=FACTORY_ALIMIT_VAL_FICO2MAX_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_16,m_iAlarmlimitFICO2max);
			}
		}
		break;
	}


	m_iAlarmlimitStateFICO2max=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_FICO2MAX_8);
	if(m_iAlarmlimitStateFICO2max<AL_ON || m_iAlarmlimitStateFICO2max>AL_AUTO)
	{
		m_iAlarmlimitStateFICO2max=FACTORY_ALIMIT_STATE_FICO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MAX_8,m_iAlarmlimitStateFICO2max);
	}

	m_iAlarmlimitFICO2min=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_FICO2MIN_16);
	m_iAlarmlimitFICO2min=(int)(CTlsFloat::Round(((double)m_iAlarmlimitFICO2min)/10, 0))*10;

	if(m_iAlarmlimitFICO2min>=m_iAlarmlimitFICO2max)
		m_iAlarmlimitFICO2min=m_iAlarmlimitFICO2max-10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitFICO2min<MINRANGE_HF_NEONATAL_FICO2MIN_KPA || m_iAlarmlimitFICO2min>MAXRANGE_HF_NEONATAL_FICO2MAX_KPA)
			{
				m_iAlarmlimitFICO2min=FACTORY_ALIMIT_VAL_FICO2MIN_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_16,m_iAlarmlimitFICO2min);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitFICO2min<MINRANGE_HF_NEONATAL_FICO2MIN_VOL || m_iAlarmlimitFICO2min>MAXRANGE_HF_NEONATAL_FICO2MAX_VOL)
			{
				m_iAlarmlimitFICO2min=FACTORY_ALIMIT_VAL_FICO2MIN_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_16,m_iAlarmlimitFICO2min);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitFICO2min<MINRANGE_HF_NEONATAL_FICO2MIN_MMHG || m_iAlarmlimitFICO2min>MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG)
			{
				m_iAlarmlimitFICO2min=FACTORY_ALIMIT_VAL_FICO2MIN_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_16,m_iAlarmlimitFICO2min);
			}
		}
		break;
	}


	m_iAlarmlimitStateFICO2min=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_FICO2MIN_8);
	if(m_iAlarmlimitStateFICO2min<AL_ON || m_iAlarmlimitStateFICO2min>AL_AUTO)
	{
		m_iAlarmlimitStateFICO2min=FACTORY_ALIMIT_STATE_FICO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MIN_8,m_iAlarmlimitStateFICO2min);
	}
	//###################################################

	m_iAlarmlimitMVmax=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_MVMAX_16);
	if(m_iAlarmlimitMVmax<50 || m_iAlarmlimitMVmax>10000)
	{
		m_iAlarmlimitMVmax=FACTORY_ALIMIT_VAL_MVMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMAX_16,m_iAlarmlimitMVmax);
	}

	m_iAlarmlimitStateMVmax=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_MVMAX_8);
	if(m_iAlarmlimitStateMVmax==AL_OFF)
		m_iAlarmlimitStateMVmax=AL_ON;
	if(m_iAlarmlimitStateMVmax<AL_ON || m_iAlarmlimitStateMVmax>AL_AUTO)
	{
		m_iAlarmlimitStateMVmax=FACTORY_ALIMIT_STATE_MVMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMAX_8,m_iAlarmlimitStateMVmax);
	}

	m_iAlarmlimitMVmin=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_MVMIN_16);
	
	if(m_iAlarmlimitMVmin<0 || m_iAlarmlimitMVmin>10000)
	{
		m_iAlarmlimitMVmin=FACTORY_ALIMIT_VAL_MVMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMIN_16,m_iAlarmlimitMVmin);
	}

	m_iAlarmlimitStateMVmin=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_MVMIN_8);
	//rku MVLOW
	//if(m_iAlarmlimitStateMVmin==AL_OFF)
	//	m_iAlarmlimitStateMVmin=AL_ON;
	if(m_iAlarmlimitStateMVmin<AL_ON || m_iAlarmlimitStateMVmin>AL_AUTO)
	{
		m_iAlarmlimitStateMVmin=FACTORY_ALIMIT_STATE_MVMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMIN_8,m_iAlarmlimitStateMVmin);
	}

	m_iAlarmlimitPIPmax=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PIPMAX_16);
	if(m_iAlarmlimitPIPmax<10 || m_iAlarmlimitPIPmax>900)
	{
		m_iAlarmlimitPIPmax=FACTORY_ALIMIT_VAL_PIPMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PIPMAX_16,m_iAlarmlimitPIPmax);
	}

	m_iAlarmlimitStatePIPmax=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PIPMAX_8);
	if(m_iAlarmlimitStatePIPmax==AL_OFF)
		m_iAlarmlimitStatePIPmax=AL_ON;
	if(m_iAlarmlimitStatePIPmax<AL_ON || m_iAlarmlimitStatePIPmax>AL_AUTO)
	{
		m_iAlarmlimitStatePIPmax=FACTORY_ALIMIT_STATE_PIPMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PIPMAX_8,m_iAlarmlimitStatePIPmax);
	}

	



	m_iAlarmlimitPEEPmin=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PEEP_16);
	if(m_iAlarmlimitPEEPmin<-100 || m_iAlarmlimitPEEPmin>900)
	{
		m_iAlarmlimitPEEPmin=FACTORY_ALIMIT_VAL_PEEP;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_16,m_iAlarmlimitPEEPmin);
	}
	m_iAlarmlimitStatePEEPmin=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PEEP_8);
	if(m_iAlarmlimitStatePEEPmin==AL_OFF)
		m_iAlarmlimitStatePEEPmin=AL_ON;
	if(m_iAlarmlimitStatePEEPmin<AL_ON || m_iAlarmlimitStatePEEPmin>AL_AUTO)
	{
		m_iAlarmlimitStatePEEPmin=FACTORY_ALIMIT_STATE_PEEP;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_8,m_iAlarmlimitStatePEEPmin);
	}


	m_iAlarmlimitPIPmin=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PIPMIN_16);
	if(m_iAlarmlimitPIPmin<m_iAlarmlimitPEEPmin || m_iAlarmlimitPIPmin>m_iAlarmlimitPIPmax-10)
	{
		m_iAlarmlimitPIPmin=FACTORY_ALIMIT_VAL_PIPMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PIPMIN_16,m_iAlarmlimitPIPmin);
	}
	m_iAlarmlimitStatePIPmin=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PIPMIN_8);
	/*if(m_iAlarmlimitStatePIPmin==AL_OFF || m_iAlarmlimitStatePIPmin==AL_AUTO)
		m_iAlarmlimitStatePIPmin=AL_ON;*/
	if(m_iAlarmlimitStatePIPmin<AL_ON || m_iAlarmlimitStatePIPmin>AL_AUTO)
	{
		m_iAlarmlimitStatePIPmin=FACTORY_ALIMIT_STATE_PIPMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PIPMIN_8,m_iAlarmlimitStatePIPmin);
	}

	m_iAlarmlimitBPMmax=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_BPMMAX_16);
	if(m_iAlarmlimitBPMmax<10 || m_iAlarmlimitBPMmax>220)
	{
		m_iAlarmlimitBPMmax=FACTORY_ALIMIT_VAL_BPMMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_BPMMAX_16,m_iAlarmlimitBPMmax);
	}

	m_iAlarmlimitStateBPMmax=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_BPMMAX_8);
	if(m_iAlarmlimitStateBPMmax<AL_ON || m_iAlarmlimitStateBPMmax>AL_AUTO)
	{
		m_iAlarmlimitStateBPMmax=FACTORY_ALIMIT_STATE_BPMMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_BPMMAX_8,m_iAlarmlimitStateBPMmax);
	}

	m_iAlimitLeakmax=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_LEAKMAX_16);
	if(m_iAlimitLeakmax<0 || m_iAlimitLeakmax>50)
	{
		m_iAlimitLeakmax=FACTORY_ALIMIT_VAL_LEAKMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_LEAKMAX_16,m_iAlimitLeakmax);
	}

	m_iAlarmlimitStateLeakmax=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_LEAKMAX_8);
	if(m_iAlarmlimitStateLeakmax<AL_ON || m_iAlarmlimitStateLeakmax>AL_AUTO)
	{
		m_iAlarmlimitStateLeakmax=FACTORY_ALIMIT_STATE_LEAKMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_LEAKMAX_8,m_iAlarmlimitStateLeakmax);
	}

	m_iAlarmlimitApnoe=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_APNOE_16);
	if(m_iAlarmlimitApnoe<MINRANGE_NEONATAL_APNOE || m_iAlarmlimitApnoe>MAXRANGE_NEONATAL_APNOE)
	{
		m_iAlarmlimitApnoe=FACTORY_ALIMIT_VAL_APNOE;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_APNOE_16,m_iAlarmlimitApnoe);
	}

	m_iAlarmlimitStateApnoe=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_APNOE_8);
	if(m_iAlarmlimitStateApnoe<AL_ON || m_iAlarmlimitStateApnoe>AL_AUTO)
	{
		m_iAlarmlimitStateApnoe=FACTORY_ALIMIT_STATE_APNOE;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_APNOE_8,m_iAlarmlimitStateApnoe);
	}

	
	m_iAlarmlimitSPO2maxHF=0;
	m_iAlarmlimitSPO2maxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2MAX_HF_16);
	if(m_iAlarmlimitSPO2maxHF<0 || m_iAlarmlimitSPO2maxHF>1000)
	{
		m_iAlarmlimitSPO2maxHF=FACTORY_ALIMIT_VAL_SPO2MAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_HF_16,m_iAlarmlimitSPO2maxHF);
	}

	m_iAlarmlimitStateSPO2maxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2MAX_HF_8);
	//if(m_iAlarmlimitStateSPO2maxHF==AL_OFF)
	//	m_iAlarmlimitStateSPO2maxHF=AL_ON;
	if(m_iAlarmlimitStateSPO2maxHF<AL_ON || m_iAlarmlimitStateSPO2maxHF>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2maxHF=FACTORY_ALIMIT_STATE_SPO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MAX_HF_8,m_iAlarmlimitStateSPO2maxHF);
	}


	m_iAlarmlimitSPO2minHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2MIN_HF_16);
	if(m_iAlarmlimitSPO2minHF<0 || m_iAlarmlimitSPO2minHF>1000)
	{
		m_iAlarmlimitSPO2minHF=FACTORY_ALIMIT_VAL_SPO2MIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_HF_16,m_iAlarmlimitSPO2minHF);
	}

	m_iAlarmlimitStateSPO2minHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2MIN_HF_8);
	//if(m_iAlarmlimitStateSPO2minHF==AL_OFF)
	//	m_iAlarmlimitStateSPO2minHF=AL_ON;
	if(m_iAlarmlimitStateSPO2minHF<AL_ON || m_iAlarmlimitStateSPO2minHF>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2minHF=FACTORY_ALIMIT_STATE_SPO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MIN_HF_8,m_iAlarmlimitStateSPO2minHF);
	}

	m_iAlarmlimitPulseRatemaxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PULSERATEMAX_HF_16);
	if(m_iAlarmlimitPulseRatemaxHF<0 || m_iAlarmlimitPulseRatemaxHF>240)
	{
		m_iAlarmlimitPulseRatemaxHF=FACTORY_ALIMIT_VAL_PULSERATEMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_HF_16,m_iAlarmlimitPulseRatemaxHF);
	}

	m_iAlarmlimitStatePulseRatemaxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PULSERATEMAX_HF_8);
	//if(m_iAlarmlimitStatePulseRatemaxHF==AL_OFF)
	//	m_iAlarmlimitStatePulseRatemaxHF=AL_ON;
	if(m_iAlarmlimitStatePulseRatemaxHF<AL_ON || m_iAlarmlimitStatePulseRatemaxHF>AL_AUTO)
	{
		m_iAlarmlimitStatePulseRatemaxHF=FACTORY_ALIMIT_STATE_PULSERATEMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMAX_HF_8,m_iAlarmlimitStatePulseRatemaxHF);
	}

	m_iAlarmlimitPulseRateminHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PULSERATEMIN_HF_16);
	if(m_iAlarmlimitPulseRateminHF<0 || m_iAlarmlimitPulseRateminHF>240)
	{
		m_iAlarmlimitPulseRateminHF=FACTORY_ALIMIT_VAL_PULSERATEMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_HF_16,m_iAlarmlimitPulseRateminHF);
	}

	m_iAlarmlimitStatePulseRateminHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PULSERATEMIN_HF_8);
	//if(m_iAlarmlimitStatePulseRateminHF==AL_OFF)
	//	m_iAlarmlimitStatePulseRateminHF=AL_ON;
	if(m_iAlarmlimitStatePulseRateminHF<AL_ON || m_iAlarmlimitStatePulseRateminHF>AL_AUTO)
	{
		m_iAlarmlimitStatePulseRateminHF=FACTORY_ALIMIT_STATE_PULSERATEMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMIN_HF_8,m_iAlarmlimitStatePulseRateminHF);
	}


	m_iAlarmlimitSPO2_PIminHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2_PIMIN_HF_16);
	if(m_iAlarmlimitSPO2_PIminHF<0 || m_iAlarmlimitSPO2_PIminHF>20000)
	{
		m_iAlarmlimitSPO2_PIminHF=FACTORY_ALIMIT_VAL_SPO2_PIMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_HF_16,m_iAlarmlimitSPO2_PIminHF);
	}

	m_iAlarmlimitStateSPO2_PIminHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2_PIMIN_HF_8);
	if(m_iAlarmlimitStateSPO2_PIminHF<AL_ON || m_iAlarmlimitStateSPO2_PIminHF>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2_PIminHF=FACTORY_ALIMIT_STATE_SPO2_PIMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_PIMIN_HF_8,m_iAlarmlimitStateSPO2_PIminHF);
	}





	m_iAlarmlimitSPO2_SIQminHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_SPO2_SIQMIN_HF_16);
	if(m_iAlarmlimitSPO2_SIQminHF<0 || m_iAlarmlimitSPO2_SIQminHF>100)
	{
		m_iAlarmlimitSPO2_SIQminHF=FACTORY_ALIMIT_VAL_SPO2_SIQMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_SIQMIN_HF_16,m_iAlarmlimitSPO2_SIQminHF);
	}

	m_iAlarmlimitStateSPO2_SIQminHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_SPO2_SIQMIN_HF_8);
	if(m_iAlarmlimitStateSPO2_SIQminHF<AL_ON || m_iAlarmlimitStateSPO2_SIQminHF>AL_AUTO)
	{
		m_iAlarmlimitStateSPO2_SIQminHF=FACTORY_ALIMIT_STATE_SPO2_SIQMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_SIQMIN_HF_8,m_iAlarmlimitStateSPO2_SIQminHF);
	}






	m_iAlarmlimitETCO2maxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MAX_HF_16);
	m_iAlarmlimitETCO2maxHF=(int)(CTlsFloat::Round(((double)m_iAlarmlimitETCO2maxHF)/10, 0))*10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitETCO2maxHF<MINRANGE_HF_NEONATAL_ETCO2MIN_KPA || m_iAlarmlimitETCO2maxHF>MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA)
			{
				m_iAlarmlimitETCO2maxHF=FACTORY_ALIMIT_VAL_ETCO2MAX_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_HF_16,m_iAlarmlimitETCO2maxHF);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitETCO2maxHF<MINRANGE_HF_NEONATAL_ETCO2MIN_VOL || m_iAlarmlimitETCO2maxHF>MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL)
			{
				m_iAlarmlimitETCO2maxHF=FACTORY_ALIMIT_VAL_ETCO2MAX_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_HF_16,m_iAlarmlimitETCO2maxHF);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitETCO2maxHF<MINRANGE_HF_NEONATAL_ETCO2MIN_MMHG || m_iAlarmlimitETCO2maxHF>MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG)
			{
				m_iAlarmlimitETCO2maxHF=FACTORY_ALIMIT_VAL_ETCO2MAX_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_HF_16,m_iAlarmlimitETCO2maxHF);
			}
		}
		break;
	}
	

	m_iAlarmlimitStateETCO2maxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_ETCO2MAX_HF_8);
	if(m_iAlarmlimitStateETCO2maxHF<AL_ON || m_iAlarmlimitStateETCO2maxHF>AL_AUTO)
	{
		m_iAlarmlimitStateETCO2maxHF=FACTORY_ALIMIT_STATE_ETCO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MAX_HF_8,m_iAlarmlimitStateETCO2maxHF);
	}

	m_iAlarmlimitETCO2minHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_ETCO2MIN_HF_16);
	m_iAlarmlimitETCO2minHF=(int)(CTlsFloat::Round(((double)m_iAlarmlimitETCO2minHF)/10, 0))*10;
	//m_iAlarmlimitETCO2minHF=(m_iAlarmlimitETCO2minHF/10)*10

	if(m_iAlarmlimitETCO2minHF>=m_iAlarmlimitETCO2maxHF)
		m_iAlarmlimitETCO2minHF=m_iAlarmlimitETCO2maxHF-10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitETCO2minHF<MINRANGE_HF_NEONATAL_ETCO2MIN_KPA || m_iAlarmlimitETCO2minHF>MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA)
			{
				m_iAlarmlimitETCO2minHF=FACTORY_ALIMIT_VAL_ETCO2MIN_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_HF_16,m_iAlarmlimitETCO2minHF);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitETCO2minHF<MINRANGE_HF_NEONATAL_ETCO2MIN_VOL || m_iAlarmlimitETCO2minHF>MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL)
			{
				m_iAlarmlimitETCO2minHF=FACTORY_ALIMIT_VAL_ETCO2MIN_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_HF_16,m_iAlarmlimitETCO2minHF);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitETCO2minHF<MINRANGE_HF_NEONATAL_ETCO2MIN_MMHG || m_iAlarmlimitETCO2minHF>MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG)
			{
				m_iAlarmlimitETCO2minHF=FACTORY_ALIMIT_VAL_ETCO2MIN_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_HF_16,m_iAlarmlimitETCO2minHF);
			}
		}
		break;
	}

	m_iAlarmlimitStateETCO2minHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_ETCO2MIN_HF_8);
	if(m_iAlarmlimitStateETCO2minHF<AL_ON || m_iAlarmlimitStateETCO2minHF>AL_AUTO)
	{
		m_iAlarmlimitStateETCO2minHF=FACTORY_ALIMIT_STATE_ETCO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MIN_HF_8,m_iAlarmlimitStateETCO2minHF);
	}

	//##########################################################
	m_iAlarmlimitFICO2maxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_FICO2MAX_HF_16);
	m_iAlarmlimitFICO2maxHF=(int)(CTlsFloat::Round(((double)m_iAlarmlimitFICO2maxHF)/10, 0))*10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitFICO2maxHF<MINRANGE_HF_NEONATAL_FICO2MIN_KPA || m_iAlarmlimitFICO2maxHF>MAXRANGE_HF_NEONATAL_FICO2MAX_KPA)
			{
				m_iAlarmlimitFICO2maxHF=FACTORY_ALIMIT_VAL_FICO2MAX_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_HF_16,m_iAlarmlimitFICO2maxHF);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitFICO2maxHF<MINRANGE_HF_NEONATAL_FICO2MIN_VOL || m_iAlarmlimitFICO2maxHF>MAXRANGE_HF_NEONATAL_FICO2MAX_VOL)
			{
				m_iAlarmlimitFICO2maxHF=FACTORY_ALIMIT_VAL_FICO2MAX_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_HF_16,m_iAlarmlimitFICO2maxHF);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitFICO2maxHF<MINRANGE_HF_NEONATAL_FICO2MIN_MMHG || m_iAlarmlimitFICO2maxHF>MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG)
			{
				m_iAlarmlimitFICO2maxHF=FACTORY_ALIMIT_VAL_FICO2MAX_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_HF_16,m_iAlarmlimitFICO2maxHF);
			}
		}
		break;
	}


	m_iAlarmlimitStateFICO2maxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_FICO2MAX_HF_8);
	if(m_iAlarmlimitStateFICO2maxHF<AL_ON || m_iAlarmlimitStateFICO2maxHF>AL_AUTO)
	{
		m_iAlarmlimitStateFICO2maxHF=FACTORY_ALIMIT_STATE_FICO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MAX_HF_8,m_iAlarmlimitStateFICO2maxHF);
	}

	m_iAlarmlimitFICO2minHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_FICO2MIN_HF_16);
	m_iAlarmlimitFICO2minHF=(int)(CTlsFloat::Round(((double)m_iAlarmlimitFICO2minHF)/10, 0))*10;
	
	if(m_iAlarmlimitFICO2minHF>=m_iAlarmlimitFICO2maxHF)
		m_iAlarmlimitFICO2minHF=m_iAlarmlimitFICO2maxHF-10;
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			if(m_iAlarmlimitFICO2minHF<MINRANGE_HF_NEONATAL_FICO2MIN_KPA || m_iAlarmlimitFICO2minHF>MAXRANGE_HF_NEONATAL_FICO2MAX_KPA)
			{
				m_iAlarmlimitFICO2minHF=FACTORY_ALIMIT_VAL_FICO2MIN_KPA;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_HF_16,m_iAlarmlimitFICO2minHF);
			}
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			if(m_iAlarmlimitFICO2minHF<MINRANGE_HF_NEONATAL_FICO2MIN_VOL || m_iAlarmlimitFICO2minHF>MAXRANGE_HF_NEONATAL_FICO2MAX_VOL)
			{
				m_iAlarmlimitFICO2minHF=FACTORY_ALIMIT_VAL_FICO2MIN_VOL;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_HF_16,m_iAlarmlimitFICO2minHF);
			}
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			if(m_iAlarmlimitFICO2minHF<MINRANGE_HF_NEONATAL_FICO2MIN_MMHG || m_iAlarmlimitFICO2minHF>MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG)
			{
				m_iAlarmlimitFICO2minHF=FACTORY_ALIMIT_VAL_FICO2MIN_MMHG;
				getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_HF_16,m_iAlarmlimitFICO2minHF);
			}
		}
		break;
	}

	m_iAlarmlimitStateFICO2minHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_FICO2MIN_HF_8);
	if(m_iAlarmlimitStateFICO2minHF<AL_ON || m_iAlarmlimitStateFICO2minHF>AL_AUTO)
	{
		m_iAlarmlimitStateFICO2minHF=FACTORY_ALIMIT_STATE_FICO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MIN_HF_8,m_iAlarmlimitStateFICO2minHF);
	}
	//##############################################################
	m_iAlarmlimitMAPmaxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_MAPMAX_HF_16);
	if(m_iAlarmlimitMAPmaxHF<50 || m_iAlarmlimitMAPmaxHF>10000)
	{
		m_iAlarmlimitMAPmaxHF=FACTORY_ALIMIT_VAL_MAPMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MAPMAX_HF_16,m_iAlarmlimitMAPmaxHF);
	}

	m_iAlarmlimitStateMAPmaxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_MAPMAX_HF_8);
	if(m_iAlarmlimitStateMAPmaxHF<AL_ON || m_iAlarmlimitStateMAPmaxHF>AL_AUTO)
	{
		m_iAlarmlimitStateMAPmaxHF=FACTORY_ALIMIT_STATE_MAPMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MAPMAX_HF_8,m_iAlarmlimitStateMAPmaxHF);
	}

	m_iAlarmlimitMAPminHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_MAPMIN_HF_16);
	if(m_iAlarmlimitMAPminHF<0 || m_iAlarmlimitMAPminHF>10000)
	{
		m_iAlarmlimitMAPminHF=FACTORY_ALIMIT_VAL_MAPMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MAPMIN_HF_16,m_iAlarmlimitMAPminHF);
	}

	m_iAlarmlimitStateMAPminHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_MAPMIN_HF_8);
	if(m_iAlarmlimitStateMAPminHF<AL_ON || m_iAlarmlimitStateMAPminHF>AL_AUTO)
	{
		m_iAlarmlimitStateMAPminHF=FACTORY_ALIMIT_STATE_MAPMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MAPMIN_HF_8,m_iAlarmlimitStateMAPminHF);
	}
	//#############################################################

	m_iAlarmlimitDCO2maxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_DCO2MAX_HF_16);
	if(m_iAlarmlimitDCO2maxHF<50 || m_iAlarmlimitDCO2maxHF>10000)
	{
		m_iAlarmlimitDCO2maxHF=FACTORY_ALIMIT_VAL_DCO2MAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_DCO2MAX_HF_16,m_iAlarmlimitDCO2maxHF);
	}

	m_iAlarmlimitStateDCO2maxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_DCO2MAX_HF_8);
	if(m_iAlarmlimitStateDCO2maxHF<AL_ON || m_iAlarmlimitStateDCO2maxHF>AL_AUTO)
	{
		m_iAlarmlimitStateDCO2maxHF=FACTORY_ALIMIT_STATE_DCO2MAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_DCO2MAX_HF_8,m_iAlarmlimitStateDCO2maxHF);
	}

	m_iAlarmlimitDCO2minHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_DCO2MIN_HF_16);
	if(m_iAlarmlimitDCO2minHF<0 || m_iAlarmlimitDCO2minHF>10000)
	{
		m_iAlarmlimitDCO2minHF=FACTORY_ALIMIT_VAL_DCO2MIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_DCO2MIN_HF_16,m_iAlarmlimitDCO2minHF);
	}

	m_iAlarmlimitStateDCO2minHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_DCO2MIN_HF_8);
	if(m_iAlarmlimitStateDCO2minHF<AL_ON || m_iAlarmlimitStateDCO2minHF>AL_AUTO)
	{
		m_iAlarmlimitStateDCO2minHF=FACTORY_ALIMIT_STATE_DCO2MIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_DCO2MIN_HF_8,m_iAlarmlimitStateDCO2minHF);
	}

	m_iAlarmlimitMVmaxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_MVMAX_HF_16);
	if(m_iAlarmlimitMVmaxHF<50 || m_iAlarmlimitMVmaxHF>20000)
	{
		m_iAlarmlimitMVmaxHF=FACTORY_ALIMIT_VAL_MVMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMAX_HF_16,m_iAlarmlimitMVmaxHF);
	}

	m_iAlarmlimitStateMVmaxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_MVMAX_HF_8);
	if(m_iAlarmlimitStateMVmaxHF<AL_ON || m_iAlarmlimitStateMVmaxHF>AL_AUTO)
	{
		m_iAlarmlimitStateMVmaxHF=FACTORY_ALIMIT_STATE_MVMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMAX_HF_8,m_iAlarmlimitStateMVmaxHF);
	}

	m_iAlarmlimitMVminHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_MVMIN_HF_16);
	if(m_iAlarmlimitMVminHF<0 || m_iAlarmlimitMVminHF>10000)
	{
		m_iAlarmlimitMVminHF=FACTORY_ALIMIT_VAL_MVMIN_HF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMIN_HF_16,m_iAlarmlimitMVminHF);
	}

	m_iAlarmlimitStateMVminHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_MVMIN_HF_8);
	if(m_iAlarmlimitStateMVminHF<AL_ON || m_iAlarmlimitStateMVminHF>AL_AUTO)
	{
		m_iAlarmlimitStateMVminHF=FACTORY_ALIMIT_STATE_MVMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMIN_HF_8,m_iAlarmlimitStateMVminHF);
	}

	m_iAlarmlimitPmaxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PMAX_HF_16);
	if(m_iAlarmlimitPmaxHF<10 || m_iAlarmlimitPmaxHF>MAXRANGE_HF_NEONATAL_PMAX)
	{
		m_iAlarmlimitPmaxHF=FACTORY_ALIMIT_VAL_PIPMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_HF_16,m_iAlarmlimitPmaxHF);
	}
	

	m_iAlarmlimitStatePmaxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PMAX_HF_8);
	if(m_iAlarmlimitStatePmaxHF<AL_ON || m_iAlarmlimitStatePmaxHF>AL_AUTO)
	{
		m_iAlarmlimitStatePmaxHF=FACTORY_ALIMIT_STATE_PIPMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_HF_8,m_iAlarmlimitStatePmaxHF);
	}

	m_iAlarmlimitPEEPminHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PEEP_HF_16);
	if(m_iAlarmlimitPEEPminHF<-100 || m_iAlarmlimitPEEPminHF>200)
	{
		m_iAlarmlimitPEEPminHF=FACTORY_ALIMIT_VAL_PEEP;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_HF_16,m_iAlarmlimitPEEPminHF);
	}

	m_iAlarmlimitStatePEEPminHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PEEP_HF_8);
	if(m_iAlarmlimitStatePEEPminHF<AL_ON || m_iAlarmlimitStatePEEPminHF>AL_AUTO)
	{
		m_iAlarmlimitStatePEEPminHF=FACTORY_ALIMIT_STATE_PEEP;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_HF_8,m_iAlarmlimitStatePEEPminHF);
	}

	m_iAlarmlimitBPMmaxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_BPMMAX_HF_16);
	if(m_iAlarmlimitBPMmaxHF<10 || m_iAlarmlimitBPMmaxHF>220)
	{
		m_iAlarmlimitBPMmaxHF=FACTORY_ALIMIT_VAL_BPMMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_BPMMAX_HF_16,m_iAlarmlimitBPMmaxHF);
	}

	m_iAlarmlimitStateBPMmaxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_BPMMAX_HF_8);
	if(m_iAlarmlimitStateBPMmaxHF<AL_ON || m_iAlarmlimitStateBPMmaxHF>AL_AUTO)
	{
		m_iAlarmlimitStateBPMmaxHF=FACTORY_ALIMIT_STATE_BPMMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_BPMMAX_HF_8,m_iAlarmlimitStateBPMmaxHF);
	}

	m_iAlarmlimitLeakmaxHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_LEAKMAX_HF_16);
	if(m_iAlarmlimitLeakmaxHF<0 || m_iAlarmlimitLeakmaxHF>50)
	{
		m_iAlarmlimitLeakmaxHF=FACTORY_ALIMIT_VAL_LEAKMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_LEAKMAX_HF_16,m_iAlarmlimitLeakmaxHF);
	}

	m_iAlarmlimitStateLeakmaxHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_LEAKMAX_HF_8);
	if(m_iAlarmlimitStateLeakmaxHF<AL_ON || m_iAlarmlimitStateLeakmaxHF>AL_AUTO)
	{
		m_iAlarmlimitStateLeakmaxHF=FACTORY_ALIMIT_STATE_LEAKMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_LEAKMAX_HF_8,m_iAlarmlimitStateLeakmaxHF);
	}

	m_iAlarmlimitApnoeHF=0;
	/*m_iAlarmlimitApnoeHF=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_APNOE_HF_16);
	if(m_iAlarmlimitApnoeHF<0 || m_iAlarmlimitApnoeHF>20)
	{
		m_iAlarmlimitApnoeHF=FACTORY_ALIMIT_VAL_APNOE_HF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_APNOE_HF_16,m_iAlarmlimitApnoeHF);
	}*/

	m_iAlarmlimitStateApnoeHF=AL_OFF;
	/*m_iAlarmlimitStateApnoeHF=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_APNOE_HF_8);
	if(m_iAlarmlimitStateApnoeHF<AL_ON || m_iAlarmlimitStateApnoeHF>AL_AUTO)
	{
		m_iAlarmlimitStateApnoeHF=FACTORY_ALIMIT_STATE_APNOE_HF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_APNOE_HF_8,m_iAlarmlimitStateApnoeHF);
	}*/

	m_iAlarmlimitPmaxCpap=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PMAX_CPAP_16);
	if(m_iAlarmlimitPmaxCpap<10 || m_iAlarmlimitPmaxCpap>700)
	{
		m_iAlarmlimitPmaxCpap=FACTORY_ALIMIT_VAL_PIPMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_CPAP_16,m_iAlarmlimitPmaxCpap);
	}

	m_iAlarmlimitStatePmaxCpap=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PMAX_CPAP_8);
	if(m_iAlarmlimitStatePmaxCpap<AL_ON || m_iAlarmlimitStatePmaxCpap>AL_AUTO)
	{
		m_iAlarmlimitStatePmaxCpap=FACTORY_ALIMIT_STATE_PIPMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_CPAP_8,m_iAlarmlimitStatePmaxCpap);
	}

	m_iAlarmlimitPEEPminCpap=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PEEP_CPAP_16);
	if(m_iAlarmlimitPEEPminCpap<-100 || m_iAlarmlimitPEEPminCpap>400)
	{
		m_iAlarmlimitPEEPminCpap=FACTORY_ALIMIT_VAL_PEEP;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_CPAP_16,m_iAlarmlimitPEEPminCpap);
	}

	m_iAlarmlimitStatePEEPminCpap=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PEEP_CPAP_8);
	if(m_iAlarmlimitStatePEEPminCpap<AL_ON || m_iAlarmlimitStatePEEPminCpap>AL_AUTO)
	{
		m_iAlarmlimitStatePEEPminCpap=FACTORY_ALIMIT_STATE_PEEP;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_CPAP_8,m_iAlarmlimitStatePEEPminCpap);
	}

	//NMODE
	m_iAlarmlimitPIPmaxNCPAP=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PMAX_NCPAP_16);
	if(m_iAlarmlimitPIPmaxNCPAP<10 || m_iAlarmlimitPIPmaxNCPAP>700)
	{
		m_iAlarmlimitPIPmaxNCPAP=FACTORY_ALIMIT_VAL_PIPMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_NCPAP_16,m_iAlarmlimitPIPmaxNCPAP);
	}

	m_iAlarmlimitStatePmaxNCPAP=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PMAX_NCPAP_8);
	if(m_iAlarmlimitStatePmaxNCPAP<AL_ON || m_iAlarmlimitStatePmaxNCPAP>AL_AUTO)
	{
		m_iAlarmlimitStatePmaxNCPAP=FACTORY_ALIMIT_STATE_PIPMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_NCPAP_8,m_iAlarmlimitStatePmaxNCPAP);
	}

	m_iAlarmlimitPEEPminNCPAP=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PEEP_NCPAP_16);
	if(m_iAlarmlimitPEEPminNCPAP<-50 || m_iAlarmlimitPEEPminNCPAP>200)
	{
		m_iAlarmlimitPEEPminNCPAP=FACTORY_ALIMIT_VAL_PEEP;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_NCPAP_16,m_iAlarmlimitPEEPminNCPAP);
	}

	m_iAlarmlimitStatePEEPminNCPAP=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PEEP_NCPAP_8);
	if(m_iAlarmlimitStatePEEPminNCPAP<AL_ON || m_iAlarmlimitStatePEEPminNCPAP>AL_AUTO)
	{
		m_iAlarmlimitStatePEEPminNCPAP=FACTORY_ALIMIT_STATE_PEEP;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_NCPAP_8,m_iAlarmlimitStatePEEPminNCPAP);
	}
	///////////////////////////////////////////////////////////////////////////////// 
	m_iAlarmlimitPIPmaxDUOPAP=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PMAX_DUOPAP_16);
	if(m_iAlarmlimitPIPmaxDUOPAP<10 || m_iAlarmlimitPIPmaxDUOPAP>700)
	{
		m_iAlarmlimitPIPmaxDUOPAP=FACTORY_ALIMIT_VAL_PIPMAX;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_DUOPAP_16,m_iAlarmlimitPIPmaxDUOPAP);
	}

	m_iAlarmlimitStatePIPmaxDUOPAP=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PMAX_DUOPAP_8);
	if(m_iAlarmlimitStatePIPmaxDUOPAP<AL_ON || m_iAlarmlimitStatePIPmaxDUOPAP>AL_AUTO)
	{
		m_iAlarmlimitStatePIPmaxDUOPAP=FACTORY_ALIMIT_STATE_PIPMAX;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_DUOPAP_8,m_iAlarmlimitStatePIPmaxDUOPAP);
	}

	/////////////////////////////////////////////////////////////////////////////////rku PIPLOW DUOPAP
	m_iAlarmlimitPIPminDUOPAP=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PMIN_DUOPAP_16);
	if(m_iAlarmlimitPIPminDUOPAP<10 || m_iAlarmlimitPIPminDUOPAP>700)
	{
		m_iAlarmlimitPIPminDUOPAP=FACTORY_ALIMIT_VAL_PIPMIN;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMIN_DUOPAP_16,m_iAlarmlimitPIPminDUOPAP);
	}

	m_iAlarmlimitStatePIPminDUOPAP=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PMIN_DUOPAP_8);
	if(m_iAlarmlimitStatePIPminDUOPAP<AL_ON || m_iAlarmlimitStatePIPminDUOPAP>AL_AUTO)
	{
		m_iAlarmlimitStatePIPminDUOPAP=FACTORY_ALIMIT_STATE_PIPMIN;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMIN_DUOPAP_8,m_iAlarmlimitStatePIPminDUOPAP);
	}

	m_iAlarmlimitPEEPminDUOPAP=getModel()->getI2C()->ReadConfigWord(ALIMIT_VAL_PEEP_DUOPAP_16);
	if(m_iAlarmlimitPEEPminDUOPAP<-50 || m_iAlarmlimitPEEPminDUOPAP>200)
	{
		m_iAlarmlimitPEEPminDUOPAP=FACTORY_ALIMIT_VAL_PEEP;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_DUOPAP_16,m_iAlarmlimitPEEPminDUOPAP);
	}

	m_iAlarmlimitStatePEEPminDUOPAP=getModel()->getI2C()->ReadConfigByte(ALIMIT_STATE_PEEP_DUOPAP_8);
	if(m_iAlarmlimitStatePEEPminDUOPAP<AL_ON || m_iAlarmlimitStatePEEPminDUOPAP>AL_AUTO)
	{
		m_iAlarmlimitStatePEEPminDUOPAP=FACTORY_ALIMIT_STATE_PEEP;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_DUOPAP_8,m_iAlarmlimitStatePEEPminDUOPAP);
	}

	//#####################numeric blocks######################################################
	m_iCurNumericBlock_IPPV=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_IPPV_8);
	if(m_iCurNumericBlock_IPPV<0 || m_iCurNumericBlock_IPPV>2)
	{
		m_iCurNumericBlock_IPPV=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_IPPV_8,m_iCurNumericBlock_IPPV);
	}

	m_iCurNumericBlock_SIPPV=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_SIPPV_8);
	if(m_iCurNumericBlock_SIPPV<0 || m_iCurNumericBlock_SIPPV>2)
	{
		m_iCurNumericBlock_SIPPV=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_SIPPV_8,m_iCurNumericBlock_SIPPV);
	}

	m_iCurNumericBlock_SIMV=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_SIMV_8);
	if(m_iCurNumericBlock_SIMV<0 || m_iCurNumericBlock_SIMV>2)
	{
		m_iCurNumericBlock_SIMV=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_SIMV_8,m_iCurNumericBlock_SIMV);
	}

	m_iCurNumericBlock_SIMVPSV=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_SIMVPSV_8);
	if(m_iCurNumericBlock_SIMVPSV<0 || m_iCurNumericBlock_SIMVPSV>2)
	{
		m_iCurNumericBlock_SIMVPSV=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_SIMVPSV_8,m_iCurNumericBlock_SIMVPSV);
	}

	m_iCurNumericBlock_PSV=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_PSV_8);
	if(m_iCurNumericBlock_PSV<0 || m_iCurNumericBlock_PSV>2)
	{
		m_iCurNumericBlock_PSV=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_PSV_8,m_iCurNumericBlock_PSV);
	}

	m_iCurNumericBlock_CPAP=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_CPAP_8);
	if(m_iCurNumericBlock_CPAP<0 || m_iCurNumericBlock_CPAP>2)
	{
		m_iCurNumericBlock_CPAP=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_CPAP_8,m_iCurNumericBlock_CPAP);
	}

	m_iCurNumericBlock_HFO=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_HFO_8);
	if(m_iCurNumericBlock_HFO<0 || m_iCurNumericBlock_HFO>2)
	{
		m_iCurNumericBlock_HFO=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_HFO_8,m_iCurNumericBlock_HFO);
	}

	m_iCurNumericBlock_NCPAP=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_NCPAP_8);
	if(m_iCurNumericBlock_NCPAP<0 || m_iCurNumericBlock_NCPAP>2)
	{
		m_iCurNumericBlock_NCPAP=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_NCPAP_8,m_iCurNumericBlock_NCPAP);
	}

	m_iCurNumericBlock_DUOPAP=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_DUOPAP_8);
	if(m_iCurNumericBlock_DUOPAP<0 || m_iCurNumericBlock_DUOPAP>2)
	{
		m_iCurNumericBlock_DUOPAP=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_DUOPAP_8,m_iCurNumericBlock_DUOPAP);
	}

	m_iCurNumericBlock_THERAPY=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_THERAPY_8);
	if(m_iCurNumericBlock_THERAPY<0 || m_iCurNumericBlock_THERAPY>2)
	{
		m_iCurNumericBlock_THERAPY=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_THERAPY_8,m_iCurNumericBlock_THERAPY);
	}

	//m_iCurNumericBlock_FLOWOFFCONV=0;
	m_iCurNumericBlock_FLOWOFFCONV=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_FLOWOFFCONV_8);
	if(m_iCurNumericBlock_FLOWOFFCONV<0 || m_iCurNumericBlock_FLOWOFFCONV>2)
	{
		m_iCurNumericBlock_FLOWOFFCONV=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_FLOWOFFCONV_8,m_iCurNumericBlock_FLOWOFFCONV);
	}

	m_iCurNumericBlock_FLOWOFFCPAP=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_FLOWOFFCPAP_8);
	if(m_iCurNumericBlock_FLOWOFFCPAP<0 || m_iCurNumericBlock_FLOWOFFCPAP>2)
	{
		m_iCurNumericBlock_FLOWOFFCPAP=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_FLOWOFFCPAP_8,m_iCurNumericBlock_FLOWOFFCPAP);
	}

	//m_iCurNumericBlock_FLOWOFFHFO=0;
	m_iCurNumericBlock_FLOWOFFHFO=getModel()->getI2C()->ReadConfigByte(NUMBLOCK_FLOWOFFHFO_8);
	if(m_iCurNumericBlock_FLOWOFFHFO<0 || m_iCurNumericBlock_FLOWOFFHFO>2)
	{
		m_iCurNumericBlock_FLOWOFFHFO=FACTORY_NUMBLOCK;
		getModel()->getI2C()->WriteConfigByte(NUMBLOCK_FLOWOFFHFO_8,m_iCurNumericBlock_FLOWOFFHFO);
	}

	m_eLeakCompensation=(eLeakCompensation)getModel()->getI2C()->ReadConfigByte(LEAKCOMPENSATIONOFF_8);
	if(m_eLeakCompensation<LC_LOW || m_eLeakCompensation>LC_HIGH)
	{
		m_eLeakCompensation=LC_MIDDLE;
		getModel()->getI2C()->WriteConfigByte(LEAKCOMPENSATIONOFF_8,(int)m_eLeakCompensation);
	}

	WORD iServiceYear=getModel()->getI2C()->ReadConfigWord(SERVICE_YEAR_16);
	WORD iServiceMonth=getModel()->getI2C()->ReadConfigByte(SERVICE_MONTH_8);
	WORD iServiceDay=getModel()->getI2C()->ReadConfigByte(SERVICE_DAY_8);

	if (iServiceYear<2015 || iServiceYear>2060 || iServiceMonth>12 || iServiceMonth<1 || iServiceDay>31 || iServiceDay<1)
	{
		iServiceYear=0;
		iServiceMonth=0;
		iServiceDay=0;
		theApp.WriteLog(_T("*** ServiceDateOn==false ***"));
		getModel()->getI2C()->WriteConfigWord(SERVICE_YEAR_16, iServiceYear);
		getModel()->getI2C()->WriteConfigByte(SERVICE_MONTH_8, iServiceMonth);
		getModel()->getI2C()->WriteConfigByte(SERVICE_DAY_8, iServiceDay);
		//status will be 1 (invalid)
	}
	else
	{
		theApp.WriteLog(_T("*** ServiceDateOn==true ***"));
		//status will be 0 (valid)
	}
	m_dtNextServiceDate.SetDate(iServiceYear, iServiceMonth, iServiceDay);

	WORD iAltitude=getModel()->getI2C()->ReadConfigWord(ALTITUDE_16);
	if(iAltitude<0 || iAltitude>5000)
	{
		iAltitude=300;
		getModel()->getI2C()->WriteConfigWord(ALTITUDE_16,iAltitude);
	}

	WORD iFCOR_NEO=getModel()->getI2C()->ReadConfigWord(FLOW_CORFACTOR_NEO_16);
	if(iFCOR_NEO<500 || iFCOR_NEO>1500)
	{
		iFCOR_NEO=960;
		getModel()->getI2C()->WriteConfigWord(FLOW_CORFACTOR_NEO_16,iFCOR_NEO);
	}

	WORD iFCOR_PED=getModel()->getI2C()->ReadConfigWord(FLOW_CORFACTOR_PED_16);
	if(iFCOR_PED<500 || iFCOR_PED>1500)
	{
		iFCOR_PED=960;
		getModel()->getI2C()->WriteConfigWord(FLOW_CORFACTOR_PED_16,iFCOR_PED);
	}
	

	//loadParameterSettings();
	//loadAlarmLimitSettings();

	//SETTINGS FOT CONV #################################################################
	CTlsRegistry regFOTsetting(_T("HKCU\\Software\\FabianHFO\\WorkState\\FOT"),true);

	WORD iMINRANGE_NEO_FOTconv_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("MINRANGE_NEO_FOTCONV_AMPLITUDE"), MINRANGE_NEO_FOTCONV_AMPLITUDE);
	WORD iMAXRANGE_PED_FOTconv_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("MAXRANGE_PED_FOTCONV_AMPLITUDE"), MAXRANGE_PED_FOTCONV_AMPLITUDE);
	m_iFOTconv_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("FOTCONV_AMPLITUDE"), FACTORY_FOT_CONV_AMPL);
	//m_iFOTconv_AMPLITUDE=getModel()->getI2C()->ReadConfigWord(PARA_FOT_CONV_AMPLITUDE_16);
	if(m_iFOTconv_AMPLITUDE<iMINRANGE_NEO_FOTconv_AMPLITUDE || m_iFOTconv_AMPLITUDE>iMAXRANGE_PED_FOTconv_AMPLITUDE)
	{
		m_iFOTconv_AMPLITUDE=FACTORY_FOT_CONV_AMPL;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_AMPLITUDE_16,FACTORY_FOT_CONV_AMPL);
	}
	if(m_iFOTconv_AMPLITUDE%STEP_FOT_AMPLITUDE!=0)
	{
		m_iFOTconv_AMPLITUDE=FACTORY_FOT_CONV_AMPL;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_AMPLITUDE_16,FACTORY_FOT_CONV_AMPL);
	}

	WORD iMINRANGE_NEO_FOTconv_FREQ=(WORD)regFOTsetting.ReadDWORD(_T("MINRANGE_NEO_FOTCONV_FREQ"), MINRANGE_NEO_FOTCONV_FREQ);
	WORD iMAXRANGE_PED_FOTconv_FREQ=(WORD)regFOTsetting.ReadDWORD(_T("MAXRANGE_PED_FOTCONV_FREQ"), MAXRANGE_PED_FOTCONV_FREQ);
	//m_iFOTconv_FREQ=getModel()->getI2C()->ReadConfigByte(PARA_FOT_CONV_FREQ_8);
	m_iFOTconv_FREQ=(WORD)regFOTsetting.ReadDWORD(_T("FOTCONV_FREQ"), FACTORY_FOT_CONV_FREQ);
	if(m_iFOTconv_FREQ<iMINRANGE_NEO_FOTconv_FREQ || m_iFOTconv_FREQ>iMAXRANGE_PED_FOTconv_FREQ)
	{
		m_iFOTconv_FREQ=FACTORY_FOT_CONV_FREQ;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_FREQ_8,FACTORY_FOT_CONV_FREQ);
	}
	if(m_iFOTconv_FREQ%STEP_FOT_FREQ!=0)
	{
		m_iFOTconv_FREQ=FACTORY_FOT_CONV_FREQ;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_FREQ_8,FACTORY_FOT_CONV_FREQ);
	}


	//WORD iMINRANGE_NEO_FOTconv_STEPS=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_STEPS"), MINRANGE_NEO_FOTHFO_STEPS);
	//WORD iMAXRANGE_PED_FOTconv_STEPS=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_STEPS"), MAXRANGE_PED_FOTHFO_STEPS);
	WORD iMINRANGE_NEO_FOTconv_STEPS=1;
	WORD iMAXRANGE_PED_FOTconv_STEPS=MAXSIZE_FOT_STEPS;
	m_iFOTconv_STEPS=getModel()->getI2C()->ReadConfigByte(PARA_FOT_CONV_STEPS_8);
	if(m_iFOTconv_STEPS<iMINRANGE_NEO_FOTconv_STEPS || m_iFOTconv_STEPS>iMAXRANGE_PED_FOTconv_STEPS)
	{
		m_iFOTconv_STEPS=FACTORY_FOT_CONV_STEPS;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_STEPS_8,m_iFOTconv_STEPS);
	}

	if(m_iFOTconv_STEPS%2==0)
	{
		m_iFOTconv_STEPS=FACTORY_FOT_CONV_STEPS;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_STEPS_8,m_iFOTconv_STEPS);
	}



	m_iDiffPEEPFOT=m_iFOTconv_STEPS/2;
	m_iDiffPEEPFOT=m_iDiffPEEPFOT*10;
	if(m_iDiffPEEPFOT<10)
		m_iDiffPEEPFOT=10;

	WORD iMINRANGE_PEEPSTART=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_PEEP_IPPV"), MINRANGE_NEO_PEEP_IPPV);
	WORD iMAXRANGE_PEEPEND=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PEEP_IPPV"), MAXRANGE_PED_PEEP_IPPV);

	m_iFOTconv_PEEPSTART=getModel()->getI2C()->ReadConfigWord(PARA_FOT_CONV_PEEPSTART_16);//5
	m_iFOTconv_PEEPEND=getModel()->getI2C()->ReadConfigWord(PARA_FOT_CONV_PEEPEND_16);//60


	if(m_iFOTconv_PEEPSTART<iMINRANGE_PEEPSTART || m_iFOTconv_PEEPSTART>m_iFOTconv_PEEPEND-m_iDiffPEEPFOT)
	{
		m_iFOTconv_PEEPSTART=FACTORY_FOT_CONV_PEEP_START;
		m_iFOTconv_PEEPEND=FACTORY_FOT_CONV_PEEP_END;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPSTART_16,m_iFOTconv_PEEPSTART);
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPEND_16,m_iFOTconv_PEEPEND);
	}
	if(m_iFOTconv_PEEPEND<m_iFOTconv_PEEPSTART+m_iDiffPEEPFOT || m_iFOTconv_PEEPEND>iMAXRANGE_PEEPEND)
	{
		m_iFOTconv_PEEPSTART=FACTORY_FOT_CONV_PEEP_START;
		m_iFOTconv_PEEPEND=FACTORY_FOT_CONV_PEEP_END;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPSTART_16,m_iFOTconv_PEEPSTART);
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPEND_16,m_iFOTconv_PEEPEND);
	}

	//SETTINGS FOT HFO #################################################################
	WORD iMINRANGE_NEO_FOThfo_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_AMPLITUDE"), MINRANGE_NEO_FOTHFO_AMPLITUDE);
	WORD iMAXRANGE_PED_FOThfo_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_AMPLITUDE"), MAXRANGE_PED_FOTHFO_AMPLITUDE);
	//m_iFOThfo_AMPLITUDE=getModel()->getI2C()->ReadConfigWord(PARA_FOT_HFO_AMPLITUDE_16);
	m_iFOThfo_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("FOTHFO_AMPLITUDE"), FACTORY_FOT_HFO_AMPL);
	//m_iFOThfo_AMPLITUDE=(WORD)regFOTsetting.ReadDWORD(_T("FOTHFO_AMPLITUDE"), FOTHFO_AMPLITUDE);

	if(m_iFOThfo_AMPLITUDE<iMINRANGE_NEO_FOThfo_AMPLITUDE || m_iFOThfo_AMPLITUDE>iMAXRANGE_PED_FOThfo_AMPLITUDE)
	{
		m_iFOThfo_AMPLITUDE=FACTORY_FOT_HFO_AMPL;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_AMPLITUDE_16,FACTORY_FOT_HFO_AMPL);
	}
	
	WORD iMINRANGE_NEO_FOThfo_FREQ=(WORD)regFOTsetting.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_FREQ"), MINRANGE_NEO_FOTHFO_FREQ);
	WORD iMAXRANGE_PED_FOThfo_FREQ=(WORD)regFOTsetting.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_FREQ"), MAXRANGE_PED_FOTHFO_FREQ);
	//m_iFOThfo_FREQ=getModel()->getI2C()->ReadConfigByte(PARA_FOT_HFO_FREQ_8);
	m_iFOThfo_FREQ=(WORD)regFOTsetting.ReadDWORD(_T("FOTHFO_FREQ"), FACTORY_FOT_HFO_FREQ);
	if(m_iFOThfo_FREQ<iMINRANGE_NEO_FOThfo_FREQ || m_iFOThfo_FREQ>iMAXRANGE_PED_FOThfo_FREQ)
	{
		m_iFOThfo_FREQ=FACTORY_FOT_HFO_FREQ;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_FREQ_8,FACTORY_FOT_HFO_FREQ);
	}
	
	//WORD iMINRANGE_NEO_FOThfo_STEPS=(WORD)regFOTsetting.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_STEPS"), MINRANGE_NEO_FOTHFO_STEPS);
	//WORD iMAXRANGE_PED_FOThfo_STEPS=(WORD)regFOTsetting.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_STEPS"), MAXRANGE_PED_FOTHFO_STEPS);
	WORD iMINRANGE_NEO_FOThfo_STEPS=1;
	WORD iMAXRANGE_PED_FOThfo_STEPS=MAXSIZE_FOT_STEPS;
	m_iFOThfo_STEPS=getModel()->getI2C()->ReadConfigByte(PARA_FOT_HFO_STEPS_8);
	if(m_iFOThfo_STEPS<iMINRANGE_NEO_FOThfo_STEPS || m_iFOThfo_STEPS>iMAXRANGE_PED_FOThfo_STEPS)
	{
		m_iFOThfo_STEPS=FACTORY_FOT_HFO_STEPS;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_STEPS_8,m_iFOThfo_STEPS);
	}

	if(m_iFOThfo_STEPS%2==0)
	{
		m_iFOThfo_STEPS=FACTORY_FOT_HFO_STEPS;
		getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_STEPS_8,m_iFOThfo_STEPS);
	}



	m_iDiffPmeanFOT=m_iFOThfo_STEPS/2;
	m_iDiffPmeanFOT=m_iDiffPmeanFOT*10;
	if(m_iDiffPmeanFOT<10)
		m_iDiffPmeanFOT=10;

	//WORD iMINRANGE_PMEANSTART=(WORD)regFOTsetting.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_PMEANSTART"), MINRANGE_NEO_FOTHFO_PMEANSTART);
	//WORD iMAXRANGE_PMEANEND=(WORD)regFOTsetting.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_PMEANEND"), MAXRANGE_PED_FOTHFO_PMEANEND);
	WORD iMINRANGE_PMEANSTART=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFPMEAN"), MINRANGE_NEO_HFPMEAN);
	WORD iMAXRANGE_PMEANEND=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFPMEAN"), MAXRANGE_PED_HFPMEAN);

	m_iFOThfo_PMEANSTART=getModel()->getI2C()->ReadConfigWord(PARA_FOT_HFO_PMEANSTART_16);//5
	m_iFOThfo_PMEANEND=getModel()->getI2C()->ReadConfigWord(PARA_FOT_HFO_PMEANEND_16);//60

	if(m_iFOThfo_PMEANSTART<iMINRANGE_PMEANSTART || m_iFOThfo_PMEANSTART>m_iFOThfo_PMEANEND-m_iDiffPmeanFOT)
	{
		m_iFOThfo_PMEANSTART=FACTORY_FOT_HFO_PMEAN_START;
		m_iFOThfo_PMEANEND=FACTORY_FOT_HFO_PMEAN_END;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANSTART_16,m_iFOThfo_PMEANSTART);
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANEND_16,m_iFOThfo_PMEANEND);
	}
	if(m_iFOThfo_PMEANEND<m_iFOThfo_PMEANSTART+m_iDiffPmeanFOT || m_iFOThfo_PMEANEND>iMAXRANGE_PMEANEND)
	{
		m_iFOThfo_PMEANSTART=FACTORY_FOT_HFO_PMEAN_START;
		m_iFOThfo_PMEANEND=FACTORY_FOT_HFO_PMEAN_END;
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANSTART_16,m_iFOThfo_PMEANSTART);
		getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANEND_16,m_iFOThfo_PMEANEND);
	}

	

#ifndef BACKUP_VENTILATION
	m_iParaDataPEEP_IPPV=m_iParaDataPEEP_TRIGGER;
	m_iParaDataPInsp_IPPV=m_iParaDataPInsp_TRIGGER;
	m_iParaDataPmaxVolG_IPPV=m_iParaDataPmaxVolG_TRIGGER;
	m_iParaDataITime_IPPV=m_iParaDataITime_TRIGGER;
	m_iParaDataETIME_IPPV=m_iParaDataETIME_TRIGGER;
	m_iParaDataBPM_IPPV=m_iParaDataBPM_TRIGGER;
	m_iParaDataIFlow_IPPV=m_iParaDataIFlow_TRIGGER;
	m_iParaDataRisetime_IPPV=m_iParaDataRisetime_TRIGGER;
	m_iParaDataEFlow_IPPV=m_iParaDataEFlow_TRIGGER;
	m_iParaDataVLimit_IPPV=m_iParaDataVLimit_TRIGGER;
	m_iParaDataVGarant_IPPV=m_iParaDataVGarant_TRIGGER;
	m_bParaDataVLimitOn_IPPV=m_bParaDataVLimitOn_TRIGGER;
	m_bParaDataVGarantOn_IPPV=m_bParaDataVGarantOn_TRIGGER;
#endif

	readAmpCorFactor();

	m_wOldConfigVersion=m_iConfigVersion;
}

/**********************************************************************************************//**
 * Gets board sub type
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The board sub type.
 **************************************************************************************************/

CStringW CConfiguration::GetBoardSubType()
{
	return m_szNetDCUversion;
}

/**********************************************************************************************//**
 * Gets kernel version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The kernel version.
 **************************************************************************************************/

CStringW CConfiguration::GetKernelVersion()
{
	return m_szKernel;
}

/**********************************************************************************************//**
 * Gets kernel acu version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The kernel acu version.
 **************************************************************************************************/

CStringW CConfiguration::GetKernelAcuVersion()
{
	return m_szAcuKernel;
}

/**********************************************************************************************//**
 * Gets kernel version date
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The kernel version date.
 **************************************************************************************************/

CStringW CConfiguration::GetKernelVersionDate()
{
	return m_szKernelDate;
}

/**********************************************************************************************//**
 * Graph set last view state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::GraphSetLastViewState(int state)
{
	m_GraphLastViewState=state;
	getModel()->getI2C()->WriteConfigByte(GRAPHLASTVIEWSUBSTATE_8,m_GraphLastViewState);
}

/**********************************************************************************************//**
 * Graph get last view state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::GraphGetLastViewState()
{
	return m_GraphLastViewState;
}

/**********************************************************************************************//**
 * Gets the language
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The language.
 **************************************************************************************************/

CStringW CConfiguration::getLANGUAGE()
{
	return m_szLanguageID;
}

/**********************************************************************************************//**
 * Gets language identifier
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The language identifier.
 **************************************************************************************************/

WORD CConfiguration::GetLanguageID()
{
	return m_iLanguageID;
}

/**********************************************************************************************//**
 * Gets language ID from name
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szLID	The lid.
 *
 * \return	The language ID from name.
 **************************************************************************************************/

WORD CConfiguration::GetLanguageIDfromName(CStringW szLID)
{
	WORD cLandID=0;

	if(szLID==LANGFILE_ENGLISH)
	{
		cLandID=LAN_ENGLISH;
	}
	else if(szLID==LANGFILE_GERMAN)
	{
		cLandID=LAN_GERMAN;
	}
	else if(szLID==LANGFILE_RUSSIAN)
	{
		cLandID=LAN_RUSSIAN;
	}
	else if(szLID==LANGFILE_FRENCH)
	{
		cLandID=LAN_FRENCH;
	}
	else if(szLID==LANGFILE_ITALIAN)
	{
		cLandID=LAN_ITALIAN;
	}
	else if(szLID==LANGFILE_FINNISH)
	{
		cLandID=LAN_FINNISH;
	}
	else if(szLID==LANGFILE_CHINESE)
	{
		cLandID=LAN_CHINESE;
	}
	else if(szLID==LANGFILE_JAPANESE)
	{
		cLandID=LAN_JAPANESE;
	}
	else if(szLID==LANGFILE_SPANISH)
	{
		cLandID=LAN_SPANISH;
	}
	else if(szLID==LANGFILE_PORTUGUESE)
	{
		cLandID=LAN_PORTUGUESE;
	}
	else if(szLID==LANGFILE_SLOVENSKO)
	{
		cLandID=LAN_SLOVENSKO;
	}
	else if(szLID==LANGFILE_CZECH)
	{
		cLandID=LAN_CZECH;
	}
	else if(szLID==LANGFILE_SWEDISH)
	{
		cLandID=LAN_SWEDISH;
	}
	/*else if(szLID==_T("test.lang"))
	{
		cLandID=LAN_TEST;
	}*/
	else if(szLID==LANGFILE_DUTCH)
	{
		cLandID=LAN_DUTCH;
	}
	else if(szLID==LANGFILE_POLISH)
	{
		cLandID=LAN_POLISH;
	}
	else if(szLID==LANGFILE_TURKISH)
	{
		cLandID=LAN_TURKISH;
	}
	else if(szLID==LANGFILE_AMERICAN)
	{
		cLandID=LAN_AMERICAN;
	}
	else if(szLID==LANGFILE_NORWAY)
	{
		cLandID=LAN_NORWAY;
	}
	else if(szLID==LANGFILE_HUNGARIAN)
	{
		cLandID=LAN_HUNGARIAN;
	}
	else if(szLID==LANGFILE_DANSK)
	{
		cLandID=LAN_DANSK;
	}
	else //default
	{
		cLandID=LAN_ENGLISH;
	}

	return cLandID;
}

/**********************************************************************************************//**
 * Gets language name from identifier
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iID	Zero-based index of the identifier.
 *
 * \return	The language name from identifier.
 **************************************************************************************************/

CStringW CConfiguration::GetLanguageNamefromID(WORD iID)
{
	CStringW szLandName=_T("");

	switch(iID)
	{
	case LAN_ENGLISH:
		{
			szLandName=LANGFILE_ENGLISH;
		}
		break;
	case LAN_GERMAN:
		{
			szLandName=LANGFILE_GERMAN;
		}
		break;
	case LAN_RUSSIAN:
		{
			szLandName=LANGFILE_RUSSIAN;
		}
		break;
	case LAN_FRENCH:
		{
			szLandName=LANGFILE_FRENCH;
		}
		break;
	case LAN_ITALIAN:
		{
			szLandName=LANGFILE_ITALIAN;
		}
		break;
	case LAN_FINNISH:
		{
			szLandName=LANGFILE_FINNISH;
		}
		break;
	case LAN_CHINESE:
		{
			szLandName=LANGFILE_CHINESE;
		}
		break;
	case LAN_JAPANESE:
		{
			szLandName=LANGFILE_JAPANESE;
		}
		break;
	case LAN_SPANISH:
		{
			szLandName=LANGFILE_SPANISH;
		}
		break;
	case LAN_PORTUGUESE:
		{
			szLandName=LANGFILE_PORTUGUESE;
		}
		break;
	case LAN_SLOVENSKO:
		{
			szLandName=LANGFILE_SLOVENSKO;
		}
		break;
	case LAN_CZECH:
		{
			szLandName=LANGFILE_CZECH;
		}
		break;
	case LAN_SWEDISH:
		{
			szLandName=LANGFILE_SWEDISH;
		}
		break;
	case LAN_DUTCH:
		{
			szLandName=LANGFILE_DUTCH;
		}
		break;
	case LAN_POLISH:
		{
			szLandName=LANGFILE_POLISH;
		}
		break;
	case LAN_TURKISH:
		{
			szLandName=LANGFILE_TURKISH;
		}
		break;
	case LAN_AMERICAN:
		{
			szLandName=LANGFILE_AMERICAN;
		}
		break;
	case LAN_NORWAY:
		{
			szLandName=LANGFILE_NORWAY;
		}
		break;
	case LAN_HUNGARIAN:
		{
			szLandName=LANGFILE_HUNGARIAN;
		}
		break;
	default:
		{
			szLandName=LANGFILE_ENGLISH;
		}
		break;
	}

	return szLandName;
}

/**********************************************************************************************//**
 * Sets a language
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szLID	The lid.
 **************************************************************************************************/

void CConfiguration::SetLanguage(CStringW szLID)
{
	BYTE cLandID=0;

	if( getModel()->getLANGUAGE()->IsXMLLangFile())
		cLandID=(BYTE)getModel()->getLANGUAGE()->GetLangFileID(szLID);
	else
	{
		if(szLID==LANGFILE_ENGLISH)
		{
			cLandID=LAN_ENGLISH;
		}
		else if(szLID==LANGFILE_GERMAN)
		{
			cLandID=LAN_GERMAN;
		}
		else if(szLID==LANGFILE_RUSSIAN)
		{
			cLandID=LAN_RUSSIAN;
		}
		else if(szLID==LANGFILE_FRENCH)
		{
			cLandID=LAN_FRENCH;
		}
		else if(szLID==LANGFILE_ITALIAN)
		{
			cLandID=LAN_ITALIAN;
		}
		else if(szLID==LANGFILE_FINNISH)
		{
			cLandID=LAN_FINNISH;
		}
		else if(szLID==LANGFILE_CHINESE)
		{
			cLandID=LAN_CHINESE;
		}
		else if(szLID==LANGFILE_JAPANESE)
		{
			cLandID=LAN_JAPANESE;
		}
		else if(szLID==LANGFILE_SPANISH)
		{
			cLandID=LAN_SPANISH;
		}
		else if(szLID==LANGFILE_PORTUGUESE)
		{
			cLandID=LAN_PORTUGUESE;
		}
		else if(szLID==LANGFILE_SLOVENSKO)
		{
			cLandID=LAN_SLOVENSKO;
		}
		else if(szLID==LANGFILE_CZECH)
		{
			cLandID=LAN_CZECH;
		}
		else if(szLID==LANGFILE_SWEDISH)
		{
			cLandID=LAN_SWEDISH;
		}
		/*else if(szLID==_T("test.lang"))
		{
			cLandID=LAN_TEST;
		}*/
		else if(szLID==LANGFILE_DUTCH)
		{
			cLandID=LAN_DUTCH;
		}
		else if(szLID==LANGFILE_POLISH)
		{
			cLandID=LAN_POLISH;
		}
		else if(szLID==LANGFILE_TURKISH)
		{
			cLandID=LAN_TURKISH;
		}
		else if(szLID==LANGFILE_AMERICAN)
		{
			cLandID=LAN_AMERICAN;
		}
		else if(szLID==LANGFILE_NORWAY)
		{
			cLandID=LAN_NORWAY;
		}
		else if(szLID==LANGFILE_HUNGARIAN)
		{
			cLandID=LAN_HUNGARIAN;
		}
		else if(szLID==LANGFILE_DANSK)
		{
			cLandID=LAN_DANSK;
		}
		else //default
		{
			cLandID=LAN_ENGLISH;
		}
	}

	m_szLanguageID=szLID;
	m_iLanguageID=cLandID;

	CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regLang.WriteDWORD(_T("LanguageID"), cLandID);
	regLang.FlushHKCU();

	getModel()->SetLanguageID(cLandID);

	if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
		&&	getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setLanguage((UINT)cLandID);
	}

}

/**********************************************************************************************//**
 * Gets alarm volume
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarm volume.
 **************************************************************************************************/

eAlarmLoudness CConfiguration::GetAlarmVolume()
{
	return m_eAlarmVolume;
}

/**********************************************************************************************//**
 * Sets alarm volume
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmVolume(eAlarmLoudness state)
{
	//CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);

	//regWorkState.WriteDWORD(_T("AlarmVolume"), (int)state);

	m_eAlarmVolume=state;
	getModel()->getI2C()->WriteConfigByte(ALARMVOLUME_8, (int)state);

	CTlsRegistry regVolume(_T("HKCU\\ControlPanel\\Volume"),true);
	DWORD dwValue=0;
	switch(m_eAlarmVolume)
	{
	case ALARMSILENT:
		{
			//dwValue=0x33333333;
			dwValue=0x00000000;
		}
		break;
	case ALARMMEDIUM:
		{
			dwValue=0x99999999;
		}
		break;
	case ALARMLOUD:
		{
			dwValue=0xFFFFFFFF;
		}
		break;
	}
	regVolume.WriteDWORD(_T("Volume"), dwValue);
	regVolume.FlushHKCU();
}

/**********************************************************************************************//**
 * Determines if we can use tve btb
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::useTveBTB()
{
	return m_bUseTveBTB;
}

/**********************************************************************************************//**
 * Sets use tve btb
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::setUseTveBTB(bool state)
{
	m_bUseTveBTB=state;
	if(m_bUseTveBTB)
	{
		theApp.WriteLog(_T("*** CConfiguration UseTveBTB==true ***"));
		getModel()->getI2C()->WriteConfigByte(USETVEBTB_8, 0);
	}
	else
	{
		theApp.WriteLog(_T("*** CConfiguration UseTveBTB==false ***"));
		getModel()->getI2C()->WriteConfigByte(USETVEBTB_8, 1);
	}
}

/**********************************************************************************************//**
 * Determines if we can use neo ped
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::useNeoPed()
{
	return m_bUseNeoPed;
}

/**********************************************************************************************//**
 * Sets use neo ped
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::setUseNeoPed(bool state)
{
	m_bUseNeoPed=state;
	if(m_bUseNeoPed)
	{
		theApp.WriteLog(_T("*** Patient range NEO+PED ***"));
		getModel()->getI2C()->WriteConfigByte(USENEOPED_8, 0);
	}
	else
	{
		theApp.WriteLog(_T("*** Patient range NEO ***"));
		getModel()->getI2C()->WriteConfigByte(USENEOPED_8, 1);

		if(GetVentRange()==PEDIATRIC)
		{
			SetVentRange(NEONATAL);
		}
	}


}

/**********************************************************************************************//**
 * Gets the btps
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::getBTPS()
{
	return m_bBTPSenable;
}

/**********************************************************************************************//**
 * Sets the btps
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::setBTPS(bool state)
{
	if(m_bBTPSenable!=state)
	{
		m_bBTPSenable=state;
		if(m_bBTPSenable)
			theApp.WriteLog(_T("*** CConfiguration BTPS enabled"));
		else
			theApp.WriteLog(_T("*** CConfiguration BTPS disabled"));

		getModel()->getI2C()->WriteConfigByte(BTPS_8, (BYTE)state);

		getModel()->Send_MODE_OPTION1();
	}
}

/**********************************************************************************************//**
 * Gets IE relation mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The IE relation mode.
 **************************************************************************************************/

eIERelationMode CConfiguration::GetIERelationMode()
{
	return m_eIERelationMode;
}

/**********************************************************************************************//**
 * Sets IE relation mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mode	The mode.
 **************************************************************************************************/

void CConfiguration::SetIERelationMode(eIERelationMode mode)
{
	m_eIERelationMode=mode;
	getModel()->getI2C()->WriteConfigByte(IERELATIONMODE_8, (int)mode);
}

/**********************************************************************************************//**
 * Gets co2unit
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The co2unit.
 **************************************************************************************************/

eCO2unit CConfiguration::GetCO2unit()
{
	return m_eCO2unit;
}

/**********************************************************************************************//**
 * Sets co2unit
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	unit	The unit.
 **************************************************************************************************/

void CConfiguration::SetCO2unit(eCO2unit unit)
{
	if(m_eCO2unit==unit)
		return;

	eCO2unit unitOld=m_eCO2unit;
	m_eCO2unit=unit;

	if(getModel()->getETCO2()!=NULL)
		getModel()->getETCO2()->changeCO2Unit(m_eCO2unit,unitOld);
}

//bool CConfiguration::IsCO2FeatureAvailable()
//{
//	return m_bCO2FeatureAvailable;
//}
//bool CConfiguration::IsCO2ModuleEnabled()
//{
//	return m_bCO2ModuleEnabled;
//}

/**********************************************************************************************//**
 * Gets co2 baro pressure
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The co2 baro pressure.
 **************************************************************************************************/

SHORT CConfiguration::GetCO2BaroPressure()
{
	return m_iCO2BaroPressure;
}

/**********************************************************************************************//**
 * Sets co2 baro pressure
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetCO2BaroPressure(SHORT value)
{
	m_iCO2BaroPressure=value;
	getModel()->getI2C()->WriteConfigWord(CO2BAROPRESSURE_16, value);

	if(getModel()->getETCO2()!=NULL)
		getModel()->getETCO2()->set_BaroPressure(value);

	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_BAROPRESSURE_CO2,(int)value);
	}
}

/**********************************************************************************************//**
 * Gets time pump automatic on
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The time pump automatic on.
 **************************************************************************************************/

BYTE CConfiguration::GetTimePumpAutoOn()
{
	return m_iCO2TimePumpAutoOn;
}

/**********************************************************************************************//**
 * Sets time pump automatic on
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetTimePumpAutoOn(BYTE value)
{
	m_iCO2TimePumpAutoOn=value;
	getModel()->getI2C()->WriteConfigByte(CO2TIMEPUMPAUTOON,(BYTE)m_iCO2TimePumpAutoOn);
}

/**********************************************************************************************//**
 * Query if this object is fast sat on
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if fast sat on, false if not.
 **************************************************************************************************/

bool CConfiguration::isFastSATon()
{
	return m_bFastSATon;
}

/**********************************************************************************************//**
 * Sets fast sa  t on
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bFastSATon	True to fast sat on.
 **************************************************************************************************/

void CConfiguration::setFastSATon(bool bFastSATon)
{
	m_bFastSATon=bFastSATon;
	if(m_bFastSATon)
		getModel()->getI2C()->WriteConfigByte(SPO2FASTSAT_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(SPO2FASTSAT_8, 0);

	/*if(getModel()->getSPO2module()!=NULL)
		getModel()->getSPO2module()->set_FastSATon(m_bFastSATon);*/

}

/**********************************************************************************************//**
 * Gets spo2alarm delay
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The spo2alarm delay.
 **************************************************************************************************/

eSPO2alarmdelay CConfiguration::getSPO2alarmDelay()
{
	return m_eSPO2alarmdelay;
}

/**********************************************************************************************//**
 * Sets spo2alarm delay
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	delay	The delay.
 **************************************************************************************************/

void CConfiguration::setSPO2alarmDelay(eSPO2alarmdelay delay)
{
	m_eSPO2alarmdelay=delay;
	getModel()->getI2C()->WriteConfigByte(SPO2ALARMDELAY_8,(BYTE)m_eSPO2alarmdelay);

	getModel()->getALARMHANDLER()->setSPO2alarmDelay(delay);
	/*if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_PRESSURE,(int)eUnit);
	}*/

}

/**********************************************************************************************//**
 * Gets spo2averaging time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The spo2averaging time.
 **************************************************************************************************/

eSPO2averagingtime CConfiguration::getSPO2averagingTime()
{
	return m_eSPO2averagingTime;
}

/**********************************************************************************************//**
 * Sets spo2averaging time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	avtime	The avtime.
 **************************************************************************************************/

void CConfiguration::setSPO2averagingTime(eSPO2averagingtime avtime)
{
	m_eSPO2averagingTime=avtime;
	getModel()->getI2C()->WriteConfigByte(SPO2AVERAGINGTIME_8, (BYTE)m_eSPO2averagingTime);
	/*if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_PRESSURE,(int)eUnit);
	}*/
}

/**********************************************************************************************//**
 * Gets sensitivity mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The sensitivity mode.
 **************************************************************************************************/

eSPO2sensitivitymode CConfiguration::getSensitivityMode()
{
	return m_eSPO2SensitivityMode;
}

/**********************************************************************************************//**
 * Sets sensitivity mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mode	The mode.
 **************************************************************************************************/

void CConfiguration::setSensitivityMode(eSPO2sensitivitymode mode)
{
	m_eSPO2SensitivityMode=mode;
	getModel()->getI2C()->WriteConfigByte(SPO2SENSITIVITY_8, (BYTE)m_eSPO2SensitivityMode);
	/*if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_PRESSURE,(int)eUnit);
	}*/
}

/**********************************************************************************************//**
 * Gets reference flow sensor
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The reference flow sensor.
 **************************************************************************************************/

eRefFlowSensor CConfiguration::GetRefFlowSensor()
{
	return m_eRefFlowSensor;
}

/**********************************************************************************************//**
 * Sets reference flow sensor
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mode	The mode.
 **************************************************************************************************/

void CConfiguration::SetRefFlowSensor(eRefFlowSensor mode)
{
	m_eRefFlowSensor=mode;
	getModel()->getI2C()->WriteConfigByte(REFFLOWSENSOR_8, (int)mode);
}

/**********************************************************************************************//**
 * Gets pressure unit
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The pressure unit.
 **************************************************************************************************/

ePressureUnit CConfiguration::GetPressureUnit()
{
	return m_ePressureUnit;
}

/**********************************************************************************************//**
 * Sets pressure unit
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	eUnit	The unit.
 **************************************************************************************************/

void CConfiguration::SetPressureUnit(ePressureUnit eUnit)
{
	m_ePressureUnit=eUnit;
	getModel()->getI2C()->WriteConfigByte(PRESSUREUNIT_8, (int)eUnit);

	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_PRESSURE,(int)eUnit);
	}
}

/**********************************************************************************************//**
 * Gets volume control
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The volume control.
 **************************************************************************************************/

eVolumeControl CConfiguration::GetVolumeControl()
{
	return m_eVControl;
}

/**********************************************************************************************//**
 * Sets volume control
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	eCtrl	The control.
 **************************************************************************************************/

void CConfiguration::SetVolumeControl(eVolumeControl eCtrl)
{
	m_eVControl=eCtrl;
	getModel()->getI2C()->WriteConfigByte(VOLUMECONTROL_8, (int)eCtrl);

	if(VCTRL_VGARANT==eCtrl)//rku, VGVL check
	{
		getModel()->getDATAHANDLER()->checkVGdependency();
	}
}

/**********************************************************************************************//**
 * Sets freshgas extent
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::setFreshgasExt(bool state)
{
	bool bChanged=false;

	if(m_bFreshgasExt!=state)
		bChanged=true;

	m_bFreshgasExt=state;
	if(m_bFreshgasExt)
	{
		theApp.WriteLog(_T("*** BiasFlow extern"));
	}
	else
	{
		theApp.WriteLog(_T("*** BiasFlow intern"));
	}

	if(true==bChanged && getModel()->getFOTThread())
	{
		if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
		}
	}
}

/**********************************************************************************************//**
 * Gets freshgas extent
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::getFreshgasExt()
{
	return m_bFreshgasExt;
}

/**********************************************************************************************//**
 * Queries if the bias flow is active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the bias flow is active, false if not.
 **************************************************************************************************/

bool CConfiguration::isBiasFlowActive()
{
	return m_bBiasFlow;
}

/**********************************************************************************************//**
 * Query if this object is spo2 registry enabled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if spo2 registry enabled, false if not.
 **************************************************************************************************/

bool CConfiguration::isSPO2REGISTRYenabled()
{
	return m_bSPO2registry;
}

 /**********************************************************************************************//**
  * Query if this object is co2 registry enabled
  *
  * \author	Rainer K�hner
  * \date	19.02.2018
  *
  * \return	True if co2 registry enabled, false if not.
  **************************************************************************************************/

 bool CConfiguration::isCO2REGISTRYenabled()
 {
	return m_bCO2registry;
 }

 /**********************************************************************************************//**
  * Query if this object is risetime registry enabled
  *
  * \author	Rainer K�hner
  * \date	19.02.2018
  *
  * \return	True if risetime registry enabled, false if not.
  **************************************************************************************************/

 bool CConfiguration::isRISETIMEREGISTRYenabled()
 {
	 //return false;
	 return m_bRISETIMEregistry;
 }

/**********************************************************************************************//**
 * Gets prico spo2low range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The calculated prico spo2low range.
 **************************************************************************************************/

BYTE CConfiguration::getPRICO_SPO2lowRange()
{
	return m_iPRICO_SPO2lowRange;
}

/**********************************************************************************************//**
 * Sets prico spo2low range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	SPO2low	The spo2low.
 **************************************************************************************************/

void CConfiguration::setPRICO_SPO2lowRange(BYTE SPO2low)
{
	m_iPRICO_SPO2lowRange=SPO2low;
	getModel()->getI2C()->WriteConfigByte(PRICOSPO2LOW_8,m_iPRICO_SPO2lowRange);
}

/**********************************************************************************************//**
 * Gets prico spo2high range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The calculated prico spo2high range.
 **************************************************************************************************/

BYTE CConfiguration::getPRICO_SPO2highRange()
{
	return m_iPRICO_SPO2highRange;
}

/**********************************************************************************************//**
 * Sets prico spo2high range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	SPO2high	The spo2high.
 **************************************************************************************************/

void CConfiguration::setPRICO_SPO2highRange(BYTE SPO2high)
{
	m_iPRICO_SPO2highRange=SPO2high;
	getModel()->getI2C()->WriteConfigByte(PRICOSPO2HIGH_8,m_iPRICO_SPO2highRange);
}

/**********************************************************************************************//**
 * Gets prico fio2low range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The calculated prico fio2low range.
 **************************************************************************************************/

BYTE CConfiguration::getPRICO_FIO2lowRange()
{
	return m_iPRICO_FIO2lowRange;
}

/**********************************************************************************************//**
 * Sets prico fio2low range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	FIO2low	The fio2low.
 **************************************************************************************************/

void CConfiguration::setPRICO_FIO2lowRange(BYTE FIO2low)
{
	m_iPRICO_FIO2lowRange=FIO2low;
	getModel()->getI2C()->WriteConfigByte(PRICOFIO2LOW_8,m_iPRICO_FIO2lowRange);
}

/**********************************************************************************************//**
 * Gets prico fio2high range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The calculated prico fio2high range.
 **************************************************************************************************/

BYTE CConfiguration::getPRICO_FIO2highRange()
{
	return m_iPRICO_FIO2highRange;
}

/**********************************************************************************************//**
 * Sets prico fio2high range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	FIO2high	The fio2high.
 **************************************************************************************************/

void CConfiguration::setPRICO_FIO2highRange(BYTE FIO2high)
{
	m_iPRICO_FIO2highRange=FIO2high;
	getModel()->getI2C()->WriteConfigByte(PRICOFIO2HIGH_8,m_iPRICO_FIO2highRange);
}

/**********************************************************************************************//**
 * Gets tube set
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The tube set.
 **************************************************************************************************/

eTubeSet CConfiguration::GetTubeSet()
{
	return m_eTubeSet;
}

/**********************************************************************************************//**
 * Sets tube set
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	tube	The tube.
 **************************************************************************************************/

void CConfiguration::SetTubeSet(eTubeSet tube)
{
	m_eTubeSet=tube;
	getModel()->getI2C()->WriteConfigByte(TUBESET_8, (int)tube);

}

/**********************************************************************************************//**
 * Gets tube configuration
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The tube configuration.
 **************************************************************************************************/

eTubeConfig CConfiguration::GetTubeConfig()
{
	return m_eTubeType;
}

/**********************************************************************************************//**
 * Gets flow sensor state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The flow sensor state.
 **************************************************************************************************/

eFlowSensorState CConfiguration::GetFlowSensorState()
{
	return m_eFlowSensorState;
}

/**********************************************************************************************//**
 * Sets flow sensor state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetFlowSensorState(eFlowSensorState state)
{

	if(		state==FLOWSENSOR_ON
		||	state==FLOWSENSOR_OFF
		||	state==FLOWSENSOR_MANOFF
		||	state==FLOWSENSOR_AUTOOFF)
		m_eFlowSensorState=state;
	else
		m_eFlowSensorState=FLOWSENSOR_ON;

	getModel()->getI2C()->WriteConfigByte(FLOWSENSORSTATE_8, (int)m_eFlowSensorState);
}

/**********************************************************************************************//**
 * Graph set automatic scale
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::GraphSetAutoScale(bool state)
{
	m_bGraphAutoScale=state;
	if(state)
		getModel()->getI2C()->WriteConfigByte(GRAPHAUTOSCALE_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(GRAPHAUTOSCALE_8, 0);
}

/**********************************************************************************************//**
 * Determines if we can graph is automatic scale
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GraphIsAutoScale()
{
	return m_bGraphAutoScale;
}

/**********************************************************************************************//**
 * Trend set automatic scale
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::TrendSetAutoScale(bool state)
{
	m_bTrendAutoScale=state;
	if(state)
		getModel()->getI2C()->WriteConfigByte(TRENDAUTOSCALE_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(TRENDAUTOSCALE_8, 0);
}

/**********************************************************************************************//**
 * Determines if we can trend is automatic scale
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::TrendIsAutoScale()
{
	return m_bTrendAutoScale;
}

/**********************************************************************************************//**
 * Graph set filled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::GraphSetFilled(bool state)
{
	m_bFilledGraph=state;
	if(state)
		getModel()->getI2C()->WriteConfigByte(FILLEDGRAPH_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(FILLEDGRAPH_8, 0);
}

/**********************************************************************************************//**
 * Determines if we can graph is filled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GraphIsFilled()
{
	return m_bFilledGraph;
}

/**********************************************************************************************//**
 * Trend set filled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::TrendSetFilled(bool state)
{
	m_bFilledTrend=state;
	if(state)
		getModel()->getI2C()->WriteConfigByte(FILLEDTREND_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(FILLEDTREND_8, 0);
}

/**********************************************************************************************//**
 * Determines if we can trend is filled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::TrendIsFilled()
{
	return m_bFilledTrend;
}

/**********************************************************************************************//**
 * Sets trigger option convert
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::setTriggerOption_CONV(eTriggereType type)
{
	m_eTriggerType_CONV=type;
	getModel()->getI2C()->WriteConfigByte(TRIGGERTYPECONV_8, (BYTE)type);

	DEBUGMSG(TRUE, (TEXT("setTriggerOption_CONV %d\r\n"),(int)type));
}

/**********************************************************************************************//**
 * Gets trigger option convert
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trigger option convert.
 **************************************************************************************************/

eTriggereType CConfiguration::getTriggerOption_CONV()
{
	return m_eTriggerType_CONV;
}

/**********************************************************************************************//**
 * Sets previous trigger option convert
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::setPrevTriggerOption_CONV(eTriggereType type)
{
	m_ePrevTrigger_CONV=type;
	getModel()->getI2C()->WriteConfigByte(PREVTRIGGERTYPECONV_8, (BYTE)type);

	DEBUGMSG(TRUE, (TEXT("setTriggerOption_CONV %d\r\n"),(int)type));
}

/**********************************************************************************************//**
 * Gets the previous trigger option convert
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The previous trigger option convert.
 **************************************************************************************************/

eTriggereType CConfiguration::getPrevTriggerOption_CONV()
{
	return m_ePrevTrigger_CONV;
}

/**********************************************************************************************//**
 * Sets trigger option cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::setTriggerOption_CPAP(eTriggereType type)
{
	m_eTriggerType_CPAP=type;
	getModel()->getI2C()->WriteConfigByte(TRIGGERTYPECPAP_8, (BYTE)type);

	DEBUGMSG(TRUE, (TEXT("setTriggerOption_CPAP %d\r\n"),(int)type));
}

/**********************************************************************************************//**
 * Gets trigger option cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trigger option cpap.
 **************************************************************************************************/

eTriggereType CConfiguration::getTriggerOption_CPAP()
{
	return m_eTriggerType_CPAP;
}

/**********************************************************************************************//**
 * Sets trigger option duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::setTriggerOption_DUOPAP(eTriggereType type)
{
	m_eTriggerType_DUOPAP=type;
	getModel()->getI2C()->WriteConfigByte(TRIGGERTYPEDUOPAP_8, (BYTE)type);

	DEBUGMSG(TRUE, (TEXT("setTriggerOption_DUOPAP %d\r\n"),(int)type));
}

/**********************************************************************************************//**
 * Gets trigger option duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trigger option duopap.
 **************************************************************************************************/

eTriggereType CConfiguration::getTriggerOption_DUOPAP()
{
	return m_eTriggerType_DUOPAP;
}

/**********************************************************************************************//**
 * Sets trigger option ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::setTriggerOption_NCPAP(eTriggereType type)
{
	m_eTriggerType_NCPAP=type;
	getModel()->getI2C()->WriteConfigByte(TRIGGERTYPENCPAP_8, (BYTE)type);

	DEBUGMSG(TRUE, (TEXT("setTriggerOption_NCPAP %d\r\n"),(int)type));
}

/**********************************************************************************************//**
 * Gets trigger option ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trigger option ncpap.
 **************************************************************************************************/

eTriggereType CConfiguration::getTriggerOption_NCPAP()
{
	return m_eTriggerType_NCPAP;
}

/**********************************************************************************************//**
 * Sets automatic oxy calibration
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAutoOxyCal(eAutoOxyCal state)
{
	m_eAutoOxyCal=state;
	getModel()->getI2C()->WriteConfigByte(AUTOOXYCAL_8, (eAutoOxyCal)state);

}

/**********************************************************************************************//**
 * Gets automatic oxy calibration
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The automatic oxy calibration.
 **************************************************************************************************/

eAutoOxyCal CConfiguration::GetAutoOxyCal()
{
	return m_eAutoOxyCal;
}

/**********************************************************************************************//**
 * Sets automatic screenlock time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAutoScreenlockTime(eAutoScreenlock state)
{
	m_eAutoScreenlocktime=state;
	getModel()->getI2C()->WriteConfigByte(AUTOSCREENLOCKTIME_8, (BYTE)state);

	switch(m_eAutoScreenlocktime)
	{
	case ASL_20:
		{
			theApp.WriteLog(_T("*** Screenlock ASL_20"));
		}
		break;
	case ASL_30:
		{
			theApp.WriteLog(_T("*** Screenlock ASL_30"));
		}
		break;
	case ASL_40:
		{
			theApp.WriteLog(_T("*** Screenlock ASL_40"));
		}
		break;
	case ASL_50:
		{
			theApp.WriteLog(_T("*** Screenlock ASL_50"));
		}
		break;
	case ASL_60:
		{
			theApp.WriteLog(_T("*** Screenlock ASL_60"));
		}
		break;
	}
}

/**********************************************************************************************//**
 * Gets automatic screenlock time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The automatic screenlock time.
 **************************************************************************************************/

eAutoScreenlock CConfiguration::GetAutoScreenlockTime()
{
	return m_eAutoScreenlocktime;
}

/**********************************************************************************************//**
 * Sets automatic screenlock active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetAutoScreenlockActive(bool state)
{
	theApp.SetAutoScreenlockActive(state);
	m_bAutoScreenlockActive=state;

	getModel()->getI2C()->WriteConfigByte(AUTOSCREENLOCKSTATE_8, (BYTE)state);

	if(m_bAutoScreenlockActive)
	{
		theApp.WriteLog(_T("*** AutoScreenlock enabled"));

		switch(m_eAutoScreenlocktime)
		{
		case ASL_20:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_20"));
			}
			break;
		case ASL_30:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_30"));
			}
			break;
		case ASL_40:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_40"));
			}
			break;
		case ASL_50:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_50"));
			}
			break;
		case ASL_60:
			{
				theApp.WriteLog(_T("*** Screenlock ASL_60"));
			}
			break;
		}
	}
	else
	{
		theApp.WriteLog(_T("*** AutoScreenlock disabled"));
	}

	

}

/**********************************************************************************************//**
 * Queries if the automatic screenlock is active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the automatic screenlock is active, false if not.
 **************************************************************************************************/

bool CConfiguration::IsAutoScreenlockActive()
{
	return m_bAutoScreenlockActive;
}

/**********************************************************************************************//**
 * Sets service password disabled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetServicePasswordDisabled(bool state)
{
	m_bServicePasswordDisabled=state;
	if(m_bServicePasswordDisabled)
	{
		theApp.WriteLog(_T("*** ServicePasswordDisabled==true"));
	}
	else
	{
		theApp.WriteLog(_T("*** ServicePasswordDisabled==false"));
	}
	getModel()->getI2C()->WriteConfigByte(SERVICEPSWRDDISABLED_8, (BYTE)state);
}

/**********************************************************************************************//**
 * Queries if the service password is disabled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the service password is disabled, false if not.
 **************************************************************************************************/

bool CConfiguration::IsServicePasswordDisabled()
{
	return m_bServicePasswordDisabled;
}

/**********************************************************************************************//**
 * Sets nursecall active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetNursecallActive(bool state)
{
	m_bNursecallActive=state;
	getModel()->getI2C()->WriteConfigByte(NURSECALLACTIVE_8, (BYTE)state);

	if(m_bNursecallActive)
		theApp.WriteLog(_T("***CConfiguration NurscallActive==true"));
	else
		theApp.WriteLog(_T("*** CConfiguration NurscallActive==false"));

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);
}

/**********************************************************************************************//**
 * Queries if the nursecall is active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the nursecall is active, false if not.
 **************************************************************************************************/

bool CConfiguration::IsNursecallActive()
{
	return m_bNursecallActive;
}

/**********************************************************************************************//**
 * Sets enable autolimit
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetEnableAutolimit(bool state)
{
	m_bEnableAutolimit=state;

	if(m_bEnableAutolimit)
		getModel()->getI2C()->WriteConfigByte(AUTOALARMLIMIT_8, 0);
	else
		getModel()->getI2C()->WriteConfigByte(AUTOALARMLIMIT_8, 1);

}

/**********************************************************************************************//**
 * Queries if the enable autolimit is active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the enable autolimit is active, false if not.
 **************************************************************************************************/

bool CConfiguration::IsEnableAutolimitActive()
{
	return m_bEnableAutolimit;
}

/**********************************************************************************************//**
 * Sets enable factory
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetEnableFactory(bool state)
{
	m_bEnableFactory=state;

	if(m_bEnableFactory)
		getModel()->getI2C()->WriteConfigByte(FACTORYSETTINGS_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(FACTORYSETTINGS_8, 0);

	/*if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);*/
}

/**********************************************************************************************//**
 * Queries if the enable factory is active
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the enable factory is active, false if not.
 **************************************************************************************************/

bool CConfiguration::IsEnableFactoryActive()
{
	return m_bEnableFactory;
}

/**********************************************************************************************//**
 * Sets hfo manager breath enabled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetHFOManBreathEnabled(bool state)
{
	m_bHFOManBreathEnabled=state;
	if(state)
		getModel()->getI2C()->WriteConfigByte(HFOMANBREATH_8, 1);
	else
		getModel()->getI2C()->WriteConfigByte(HFOMANBREATH_8, 0);

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);
}

/**********************************************************************************************//**
 * Queries if the hfo manager breath is enabled
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if the hfo manager breath is enabled, false if not.
 **************************************************************************************************/

bool CConfiguration::IsHFOManBreathEnabled()
{
	return m_bHFOManBreathEnabled;
}

/**********************************************************************************************//**
 * Sets E-Flow equal I-Flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CConfiguration::SetEFLOWequalILFOW(bool state)
{
	m_bEFLOWequalILFOW=state;
	if(state)
	{
		getModel()->getI2C()->WriteConfigByte(EFLOWEQUIFLOW_8, 1);
		//getModel()->getDATAHANDLER()->SetEFlowParadata_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV(),true);
		//getModel()->getDATAHANDLER()->SetEFlowParadata_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER(),true);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(EFLOWEQUIFLOW_8, 0);
		//getModel()->getDATAHANDLER()->SetEFlowParadata_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_IPPV(),true);
		//getModel()->getDATAHANDLER()->SetEFlowParadata_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER(),true);
	}

	
}

/**********************************************************************************************//**
 * Query if this object is E-Flow equal I-Flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if eflo wequal ilfow, false if not.
 **************************************************************************************************/

bool CConfiguration::IsEFLOWequalILFOW()
{
	return m_bEFLOWequalILFOW;
}

/**********************************************************************************************//**
 * Gets show E-flow in settings
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::getShowEFlowInSettings()
{
	return m_bShowEFlowInSettings;
}

/**********************************************************************************************//**
 * Sets show E-flow in settings
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::setShowEFlowInSettings(bool bState)
{
	m_bShowEFlowInSettings=bState;

	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(EFLOWSETTINGS_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(EFLOWSETTINGS_8, 0);
	}
}

/**********************************************************************************************//**
 * Gets play backup sound
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetPlayBackupSound()
{
	return m_bPlayBackupSound;
}
//void CConfiguration::SetPlayBackupSound(bool bState)
//{
//	m_bPlayBackupSound=bState;
//
//	if(bState)
//	{
//		getModel()->getI2C()->WriteConfigByte(BACKUPSOUND_8, 1);
//	}
//	else
//	{
//		getModel()->getI2C()->WriteConfigByte(BACKUPSOUND_8, 0);
//	}
//}

/**********************************************************************************************//**
 * Gets play vlimit sound
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetPlayVlimitSound()
{
	return m_bPlayVlimitSound;
}
//void CConfiguration::SetPlayVlimitSound(bool bState)
//{
//	m_bPlayVlimitSound=bState;
//
//	if(bState)
//	{
//		getModel()->getI2C()->WriteConfigByte(VLIMITSOUND_8, 1);
//		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()==AS_ACTIVE)
//		{
//			getModel()->getALARMHANDLER()->updateAlarmSound();
//		}
//	}
//	else
//	{
//		getModel()->getI2C()->WriteConfigByte(VLIMITSOUND_8, 0);
//		if(getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()!=AS_NONE)
//		{
//			getModel()->getALARMHANDLER()->updateAlarmSound();
//		}
//	}
//}

/**********************************************************************************************//**
 * Sets percent abort criterion psv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetPercentAbortCriterionPSV(int iVal)
{
	m_iPercentAbortCriterionPSV=iVal;
	getModel()->getI2C()->WriteConfigByte(ABORTCRITERIONPSV_8, iVal);
}

/**********************************************************************************************//**
 * Gets percent abort criterion psv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The percent abort criterion psv.
 **************************************************************************************************/

int CConfiguration::GetPercentAbortCriterionPSV()
{
	return m_iPercentAbortCriterionPSV;
}

/**********************************************************************************************//**
 * Sets flush time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	byTime	The by time.
 **************************************************************************************************/

void CConfiguration::SetFlushTime(BYTE byTime)
{

	m_byFlushTime=byTime;
	getModel()->getI2C()->WriteConfigByte(O2FLUSHTIME_8, byTime);
	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_FLUSHTIME,byTime);
	}
}

/**********************************************************************************************//**
 * Gets flush time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The flush time.
 **************************************************************************************************/

BYTE CConfiguration::GetFlushTime()
{
	return m_byFlushTime;
}

/**********************************************************************************************//**
 * Sets manual breath time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iTime	Zero-based index of the time.
 **************************************************************************************************/

void CConfiguration::SetManBreathTime(int iTime)
{
	
	m_iManBreathTime=iTime;
	getModel()->getI2C()->WriteConfigByte(MANBREATHTIME_8, iTime);
}

/**********************************************************************************************//**
 * Gets manual breath time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The manager breath time.
 **************************************************************************************************/

int CConfiguration::GetManBreathTime()
{
	return m_iManBreathTime;
}

/**********************************************************************************************//**
 * Sets fo tconventional amplitude
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	amp	The amplitude.
 **************************************************************************************************/

void CConfiguration::setFOTconv_AMPLITUDE(WORD amp)
{
	m_iFOTconv_AMPLITUDE=amp;
	getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_AMPLITUDE_16, amp);
}

/**********************************************************************************************//**
 * Gets fot conventional amplitude
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot conventional amplitude.
 **************************************************************************************************/

WORD CConfiguration::getFOTconv_AMPLITUDE()
{
	return m_iFOTconv_AMPLITUDE;
}

/**********************************************************************************************//**
 * Sets fot conventional frequency
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	freq	The frequency.
 **************************************************************************************************/

void CConfiguration::setFOTconv_FREQ(BYTE freq)
{
	m_iFOTconv_FREQ=freq;
	getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_FREQ_8, freq);
}

/**********************************************************************************************//**
 * Gets fot conventional frequency
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot conventional frequency.
 **************************************************************************************************/

BYTE CConfiguration::getFOTconv_FREQ()
{
	return m_iFOTconv_FREQ;
}

/**********************************************************************************************//**
 * Sets fot conventional PEEPend
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	peep	The peep.
 **************************************************************************************************/

void CConfiguration::setFOTconv_PEEPEND(WORD peep)
{
	m_iFOTconv_PEEPEND=peep;
	getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPEND_16, peep);
}

/**********************************************************************************************//**
 * Gets fot conventional PEEPend
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot conventional PEEPend.
 **************************************************************************************************/

WORD CConfiguration::getFOTconv_PEEPEND()
{
	return m_iFOTconv_PEEPEND;
}

/**********************************************************************************************//**
 * Sets fot conventional PEEPstart
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	peep	The peep.
 **************************************************************************************************/

void CConfiguration::setFOTconv_PEEPSTART(WORD peep)
{
	m_iFOTconv_PEEPSTART=peep;
	getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPSTART_16, peep);
}

/**********************************************************************************************//**
 * Gets fot conventional PEEPstart
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot conventional PEEPstart.
 **************************************************************************************************/

WORD CConfiguration::getFOTconv_PEEPSTART()
{
	return m_iFOTconv_PEEPSTART;
}

/**********************************************************************************************//**
 * Sets fot conventional steps
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	steps	The steps.
 **************************************************************************************************/

void CConfiguration::setFOTconv_STEPS(BYTE steps)
{
	m_iFOTconv_STEPS=steps;
	getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_STEPS_8, steps);
}

/**********************************************************************************************//**
 * Gets fot conventional steps
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot conventional steps.
 **************************************************************************************************/

BYTE CConfiguration::getFOTconv_STEPS()
{
	return m_iFOTconv_STEPS;
}

/**********************************************************************************************//**
 * Sets difference PEEP fot
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	diff	The difference.
 **************************************************************************************************/

void CConfiguration::setDiffPEEPFOT(WORD diff)
{
	m_iDiffPEEPFOT=diff;
}

/**********************************************************************************************//**
 * Gets difference PEEP fot
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The difference peepfot.
 **************************************************************************************************/

WORD CConfiguration::getDiffPEEPFOT()
{
	return m_iDiffPEEPFOT;
}

/**********************************************************************************************//**
 * Sets fot hfo amplitude
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	amp	The amp.
 **************************************************************************************************/

void CConfiguration::setFOThfo_AMPLITUDE(WORD amp)
{
	m_iFOThfo_AMPLITUDE=amp;
	getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_AMPLITUDE_16, amp);
}

/**********************************************************************************************//**
 * Gets fot hfo amplitude
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fo thfo amplitude.
 **************************************************************************************************/

WORD CConfiguration::getFOThfo_AMPLITUDE()
{
	return m_iFOThfo_AMPLITUDE;
}

/**********************************************************************************************//**
 * Sets fot hfo frequency
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	freq	The frequency.
 **************************************************************************************************/

void CConfiguration::setFOThfo_FREQ(BYTE freq)
{
	m_iFOThfo_FREQ=freq;
	getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_FREQ_8, freq);
}

/**********************************************************************************************//**
 * Gets fot hfo frequency
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot hfo frequency.
 **************************************************************************************************/

BYTE CConfiguration::getFOThfo_FREQ()
{
	return m_iFOThfo_FREQ;
}

/**********************************************************************************************//**
 * Sets fot hfo pmeanend
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	pmean	The pmean.
 **************************************************************************************************/

void CConfiguration::setFOThfo_PMEANEND(WORD pmean)
{
	m_iFOThfo_PMEANEND=pmean;
	getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANEND_16, pmean);
}

/**********************************************************************************************//**
 * Gets fot hfo pmeanend
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot hfo pmeanend.
 **************************************************************************************************/

WORD CConfiguration::getFOThfo_PMEANEND()
{
	return m_iFOThfo_PMEANEND;
}

/**********************************************************************************************//**
 * Sets fot hfo pmeanstart
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	pmean	The pmean.
 **************************************************************************************************/

void CConfiguration::setFOThfo_PMEANSTART(WORD pmean)
{
	m_iFOThfo_PMEANSTART=pmean;
	getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANSTART_16, pmean);
}

/**********************************************************************************************//**
 * Gets fot hfo pmeanstart
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot hfo pmeanstart.
 **************************************************************************************************/

WORD CConfiguration::getFOThfo_PMEANSTART()
{
	return m_iFOThfo_PMEANSTART;
}

/**********************************************************************************************//**
 * Sets fot hfo steps
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	steps	The steps.
 **************************************************************************************************/

void CConfiguration::setFOThfo_STEPS(BYTE steps)
{
	m_iFOThfo_STEPS=steps;
	getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_STEPS_8, steps);
}

/**********************************************************************************************//**
 * Gets fot hfo steps
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot hfo steps.
 **************************************************************************************************/

BYTE CConfiguration::getFOThfo_STEPS()
{
	return m_iFOThfo_STEPS;
}

/**********************************************************************************************//**
 * Sets difference pmean fot
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	diff	The difference.
 **************************************************************************************************/

void CConfiguration::setDiffPmeanFOT(WORD diff)
{
	m_iDiffPmeanFOT=diff;
}

/**********************************************************************************************//**
 * Gets difference pmean fot
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The difference pmean fot.
 **************************************************************************************************/

WORD CConfiguration::getDiffPmeanFOT()
{
	return m_iDiffPmeanFOT;
}

/**********************************************************************************************//**
 * Sets nebulizer time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iTime	Zero-based index of the time.
 **************************************************************************************************/

void CConfiguration::SetNebulizerTime(int iTime)
{

	m_iNebulizerTime=iTime;
	getModel()->getI2C()->WriteConfigByte(NEBULIZERTIME_8, iTime);
}

/**********************************************************************************************//**
 * Gets nebulizer time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The nebulizer time.
 **************************************************************************************************/

int CConfiguration::GetNebulizerTime()
{
	return m_iNebulizerTime;
}

/**********************************************************************************************//**
 * Gets current pmin alarm delay
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The current pmin alarm delay.
 **************************************************************************************************/

int CConfiguration::GetCurPminAlarmDelay()
{
	return m_iPminAlarmDelay;
}

/**********************************************************************************************//**
 * Sets current pmin alarm delay
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iTime	Zero-based index of the time.
 **************************************************************************************************/

void CConfiguration::SetCurPminAlarmDelay(int iTime)
{

	m_iPminAlarmDelay=iTime;
	getModel()->getI2C()->WriteConfigByte(PMINALARMDELAY_8, iTime);
}

/**********************************************************************************************//**
 * Gets current patient alarm delay
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The current patient alarm delay.
 **************************************************************************************************/

int CConfiguration::getCurPatientAlarmDelay()
{
	return m_iPatientAlarmDelay;
}

/**********************************************************************************************//**
 * Sets current patient alarm delay
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iTime	Zero-based index of the time.
 **************************************************************************************************/

void CConfiguration::setCurPatientAlarmDelay(int iTime)
{
	m_iPatientAlarmDelay=iTime;
	getModel()->getI2C()->WriteConfigByte(PATIENTALARMDELAY_8, iTime);
}

//double CConfiguration::GetMAXSCALE_CLFIO2_GRAPH()
//{
//	return (double)m_iMAXSCALE_CLFIO2_GRAPH/10;
//}
//void CConfiguration::SetMAXSCALE_CLFIO2_GRAPH(double value)
//{
//	int iTemp=(int)(value*10);
//	m_iMAXSCALE_CLFIO2_GRAPH=iTemp;
//	getModel()->getI2C()->WriteConfigWord(MAXSCALE_CLFIO2_GRAPH_16, iTemp);
//}
//double CConfiguration::GetMAXSCALE_EMG_GRAPH()
//{
//	return (double)m_iMAXSCALE_EMG_GRAPH/10;
//}
//void CConfiguration::SetMAXSCALE_EMG_GRAPH(double value)
//{
//	int iTemp=(int)(value*10);
//	m_iMAXSCALE_EMG_GRAPH=iTemp;
//	getModel()->getI2C()->WriteConfigWord(MAXSCALE_EMG_GRAPH_16, iTemp);
//}

/**********************************************************************************************//**
 * Gets maxscale spo2 graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale spo2 graph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_SPO2_GRAPH()
{
	return (double)m_iMAXSCALE_SPO2_GRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale spo2 graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_SPO2_GRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_SPO2_GRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_SPO2_GRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale co2 graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale co2 graph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_CO2_GRAPH()
{
	return (double)m_iMAXSCALE_CO2_GRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale co2 graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_CO2_GRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_CO2_GRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_CO2_GRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale flow graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale flow graph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_FLOW_GRAPH()
{
	return (double)m_iMAXSCALE_FLOW_GRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale flow graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_FLOW_GRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_FLOW_GRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_GRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale pressure graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale pressure graph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_PRESSURE_GRAPH()
{
	return (double)m_iMAXSCALE_PRESSURE_GRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale pressure graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_PRESSURE_GRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_PRESSURE_GRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_GRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale volume graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale volume graph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_VOLUME_GRAPH()
{
	return (double)m_iMAXSCALE_VOLUME_GRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale volume graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_VOLUME_GRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_VOLUME_GRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_GRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale spo2 hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale spo2 hfgraph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_SPO2_HFGRAPH()
{
	return (double)m_iMAXSCALE_SPO2_HFGRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale spo2 hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_SPO2_HFGRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_SPO2_HFGRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_SPO2_HFGRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale co2 hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale co2 hfgraph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_CO2_HFGRAPH()
{
	return (double)m_iMAXSCALE_CO2_HFGRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale co2 hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_CO2_HFGRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_CO2_HFGRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_CO2_HFGRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale flow hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale flow hfgraph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_FLOW_HFGRAPH()
{
	return (double)m_iMAXSCALE_FLOW_HFGRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale flow hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_FLOW_HFGRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_FLOW_HFGRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_HFGRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale volume hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale volume hfgraph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_VOLUME_HFGRAPH()
{
	return (double)m_iMAXSCALE_VOLUME_HFGRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale volume hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_VOLUME_HFGRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_VOLUME_HFGRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_HFGRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale pressure hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale pressure hfgraph.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_PRESSURE_HFGRAPH()
{
	return (double)m_iMAXSCALE_PRESSURE_HFGRAPH/10;
}

/**********************************************************************************************//**
 * Sets maxscale pressure hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_PRESSURE_HFGRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_PRESSURE_HFGRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_HFGRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets minscale pressure hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The minscale pressure hfgraph.
 **************************************************************************************************/

double CConfiguration::GetMINSCALE_PRESSURE_HFGRAPH()
{
	return (double)m_iMINSCALE_PRESSURE_HFGRAPH/10;
}

/**********************************************************************************************//**
 * Sets minscale pressure hfgraph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMINSCALE_PRESSURE_HFGRAPH(double value)
{
	int iTemp=(int)(value*10);
	m_iMINSCALE_PRESSURE_HFGRAPH=iTemp;
	getModel()->getI2C()->WriteConfigWord(MINSCALE_PRESSURE_HFGRAPH_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale pressure hfpvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale pressure hfpvloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_PRESSURE_HFPVLOOP()
{
	return (double)m_iMAXSCALE_PRESSURE_HFPVLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale pressure hfpvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_PRESSURE_HFPVLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_PRESSURE_HFPVLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_HFPVLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets minscale pressure hfpvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The minscale pressure hfpvloop.
 **************************************************************************************************/

double CConfiguration::GetMINSCALE_PRESSURE_HFPVLOOP()
{
	return (double)m_iMINSCALE_PRESSURE_HFPVLOOP/10;
}

/**********************************************************************************************//**
 * Sets minscale pressure hfpvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMINSCALE_PRESSURE_HFPVLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMINSCALE_PRESSURE_HFPVLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MINSCALE_PRESSURE_HFPVLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale volume hfpvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale volume hfpvloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_VOLUME_HFPVLOOP()
{
	return (double)m_iMAXSCALE_VOLUME_HFPVLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale volume hfpvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_VOLUME_HFPVLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_VOLUME_HFPVLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_HFPVLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale flow hfvfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale flow hfvfloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_FLOW_HFVFLOOP()
{
	return (double)m_iMAXSCALE_FLOW_HFVFLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale flow hfvfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_FLOW_HFVFLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_FLOW_HFVFLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_HFVFLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale volume hfvfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale volume hfvfloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_VOLUME_HFVFLOOP()
{
	return (double)m_iMAXSCALE_VOLUME_HFVFLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale volume hfvfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_VOLUME_HFVFLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_VOLUME_HFVFLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_HFVFLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale pressure pvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale pressure pvloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_PRESSURE_PVLOOP()
{
	return (double)m_iMAXSCALE_PRESSURE_PVLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale pressure pvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_PRESSURE_PVLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_PRESSURE_PVLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_PRESSURE_PVLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale volume pvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale volume pvloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_VOLUME_PVLOOP()
{
	return (double)m_iMAXSCALE_VOLUME_PVLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale volume pvloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_VOLUME_PVLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_VOLUME_PVLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_PVLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale volume vfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale volume vfloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_VOLUME_VFLOOP()
{
	return (double)m_iMAXSCALE_VOLUME_VFLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale volume vfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_VOLUME_VFLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_VOLUME_VFLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_VOLUME_VFLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale flow vfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale flow vfloop.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_FLOW_VFLOOP()
{
	return (double)m_iMAXSCALE_FLOW_VFLOOP/10;
}

/**********************************************************************************************//**
 * Sets maxscale flow vfloop
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_FLOW_VFLOOP(double value)
{
	int iTemp=(int)(value*10);
	m_iMAXSCALE_FLOW_VFLOOP=iTemp;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_FLOW_VFLOOP_16, iTemp);
}

/**********************************************************************************************//**
 * Gets maxscale fot pressure
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale fot pressure.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_FOT_PRESSURE()
{
	return (double)m_iMAXSCALE_FOT_PRESSURE;
}

/**********************************************************************************************//**
 * Sets maxscale fot pressure
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_FOT_PRESSURE(double value)
{
	m_iMAXSCALE_FOT_PRESSURE=value;
	getModel()->getI2C()->WriteConfigWord(MAXSCALE_FOT_PRESSURE_16, value);
}

/**********************************************************************************************//**
 * Gets minscale fot xrs
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The minscale fot xrs.
 **************************************************************************************************/

double CConfiguration::GetMINSCALE_FOT_XRS()
{
	return m_iMINSCALE_FOT_XRS;
}

/**********************************************************************************************//**
 * Sets minscale fot xrs
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMINSCALE_FOT_XRS(double value)
{
	m_iMINSCALE_FOT_XRS=value;
	//getModel()->getI2C()->WriteConfigWord(MINSCALE_FOT_XRS_16, value); //rku, check FOTGRAPH
}

/**********************************************************************************************//**
 * Gets maxscale fot xrs
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The maxscale fot xrs.
 **************************************************************************************************/

double CConfiguration::GetMAXSCALE_FOT_XRS()
{
	return m_iMAXSCALE_FOT_XRS;
}

/**********************************************************************************************//**
 * Sets maxscale fot xrs
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetMAXSCALE_FOT_XRS(double value)
{
	m_iMAXSCALE_FOT_XRS=value;
	//getModel()->getI2C()->WriteConfigWord(MINSCALE_FOT_XRS_16, value); //rku, check FOTGRAPH
}

/**********************************************************************************************//**
 * Trend set Pinsp maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetPInspMax(int value)
{
	m_iTrendPInspMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_PINSP_16, value);
}

/**********************************************************************************************//**
 * Trend get Pinsp maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetPInspMax()
{
	return m_iTrendPInspMax;
}

/**********************************************************************************************//**
 * Trend set Pmean maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetPMeanMax(int value)
{
	m_iTrendPMeanMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_PMEAN_16, value);
}

/**********************************************************************************************//**
 * Trend get Pmean maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetPMeanMax()
{
	return m_iTrendPMeanMax;
}

/**********************************************************************************************//**
 * Trend set fio2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetFIO2Max(int value)
{
	m_iTrendFIO2Max=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_FIO2_16, value);
}

/**********************************************************************************************//**
 * Trend get fio2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetFIO2Max()
{
	return m_iTrendFIO2Max;
}

/**********************************************************************************************//**
 * Trend set vte maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetVteMax(int value)
{
	m_iTrendVteMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_VTE_16, value);
}

/**********************************************************************************************//**
 * Trend get vte maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetVteMax()
{
	return m_iTrendVteMax;
}

/**********************************************************************************************//**
 * Trend set compliance maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetComplianceMax(int value)
{
	m_iTrendComplianceMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_COMPLIANCE_16, value);
}

/**********************************************************************************************//**
 * Trend get compliance maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetComplianceMax()
{
	return m_iTrendComplianceMax;
}

/**********************************************************************************************//**
 * Trend set dco2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetDCO2Max(int value)
{
	m_iTrendDCO2Max=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_DCO2_16, value);
}

/**********************************************************************************************//**
 * Trend get dco2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetDCO2Max()
{
	return m_iTrendDCO2Max;
}

/**********************************************************************************************//**
 * Trend set mv maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetMVMax(int value)
{
	m_iTrendMVMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_MV_16, value);
}

/**********************************************************************************************//**
 * Trend get mv maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetMVMax()
{
	return m_iTrendMVMax;
}

/**********************************************************************************************//**
 * Trend set hfamp maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetHFAMPMax(int value)
{
	m_iTrendHFAMPMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_HFAMP_16, value);
}

/**********************************************************************************************//**
 * Trend get hfamp maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetHFAMPMax()
{
	return m_iTrendHFAMPMax;
}

/**********************************************************************************************//**
 * Trend set rsbi maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetRSBIMax(int value)
{
	m_iTrendRSBIMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_RSBI_16, value);
}

/**********************************************************************************************//**
 * Trend get rsbi maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetRSBIMax()
{
	return m_iTrendRSBIMax;
}

/**********************************************************************************************//**
 * Trend set sharemvmand maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetSHAREMVMANDMax(int value)
{
	m_iTrendSHAREMVMANDMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SHAREMVMAND_16, value);
}

/**********************************************************************************************//**
 * Trend get sharemvmand maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetSHAREMVMANDMax()
{
	return m_iTrendSHAREMVMANDMax;
}

/**********************************************************************************************//**
 * Trend set resistance maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetRESISTANCEMax(int value)
{
	m_iTrendRESISTANCEMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_RESISTANCE_16, value);
}

/**********************************************************************************************//**
 * Trend get resistance maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetRESISTANCEMax()
{
	return m_iTrendRESISTANCEMax;
}

/**********************************************************************************************//**
 * Trend set leak maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetLEAKMax(int value)
{
	m_iTrendLEAKMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_LEAK_16, value);
}

/**********************************************************************************************//**
 * Trend get leak maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetLEAKMax()
{
	return m_iTrendLEAKMax;
}

/**********************************************************************************************//**
 * Trend set spo2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetSPO2Max(int value)
{
	m_iTrendSPO2Max=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SPO2_16, value);
}

/**********************************************************************************************//**
 * Trend get spo2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetSPO2Max()
{
	return m_iTrendSPO2Max;
}

/**********************************************************************************************//**
 * Trend set spo2 pulserate maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetSPO2PRMax(int value)
{
	m_iTrendSPO2PRMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SPO2PR_16, value);
}

/**********************************************************************************************//**
 * Trend get spo2 pulserate maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetSPO2PRMax()
{
	return m_iTrendSPO2PRMax;
}

/**********************************************************************************************//**
 * Trend set frequency maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetFREQUENCYMax(int value)
{
	m_iTrendFREQUENCYMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_FREQUENCY_16, value);
}

/**********************************************************************************************//**
 * Trend get frequency maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetFREQUENCYMax()
{
	return m_iTrendFREQUENCYMax;
}

/**********************************************************************************************//**
 * Trend set spo2 pi maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetSPO2PIMax(int value)
{
	m_iTrendSPO2PIMax=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_SPO2PI_16, value);
}

/**********************************************************************************************//**
 * Trend get spo2 pi maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetSPO2PIMax()
{
	return m_iTrendSPO2PIMax;
}

/**********************************************************************************************//**
 * Trend set etco2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::TrendSetETCO2Max(int value)
{
	m_iTrendETCO2Max=value;
	getModel()->getI2C()->WriteConfigWord(TRENDSCALE_ETCO2_16, value);
}

/**********************************************************************************************//**
 * Trend get etco2 maximum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::TrendGetETCO2Max()
{
	return m_iTrendETCO2Max;
}

/**********************************************************************************************//**
 * Graph set actual speed graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::GraphSetActualSpeedGraph(int value)
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_iGraphSpeedHFO=value;
		getModel()->getI2C()->WriteConfigByte(GRAPHSPEEDHFO_8, value);
	}
	else
	{
		m_iGraphSpeedNORM=value;
		getModel()->getI2C()->WriteConfigByte(GRAPHSPEEDNORM_8, value);
	}
	
}

/**********************************************************************************************//**
 * Graph get actual speed graph
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::GraphGetActualSpeedGraph()
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		return m_iGraphSpeedHFO;
	}
	else
	{
		return m_iGraphSpeedNORM;
	}
}

/**********************************************************************************************//**
 * Graph set actual speed clfio2
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::GraphSetActualSpeedCLFIO2(int value)
{
	m_iGraphSpeedCLFIO2=value;
	getModel()->getI2C()->WriteConfigByte(GRAPHSPEEDCLFIO2_8, value);

}

/**********************************************************************************************//**
 * Graph get actual speed clfio2
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::GraphGetActualSpeedCLFIO2()
{
	return m_iGraphSpeedCLFIO2;
}

/**********************************************************************************************//**
 * Sets trend span
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	span	The span.
 **************************************************************************************************/

void CConfiguration::SetTrendSpan(UINT span)
{
	m_iTrendSpan=span;
	getModel()->getI2C()->WriteConfigWord(TREND_SPAN_16, span);
}

/**********************************************************************************************//**
 * Gets trend span
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trend span.
 **************************************************************************************************/

UINT CConfiguration::GetTrendSpan()
{
	return m_iTrendSpan;
}

/**********************************************************************************************//**
 * Sets trend type 1
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::SetTrendType1(UINT type)
{
	m_iTypeTrend1=type;
	getModel()->getI2C()->WriteConfigByte(TREND_TYPE1_8, type);
}

/**********************************************************************************************//**
 * Gets trend type 1
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trend type 1.
 **************************************************************************************************/

UINT CConfiguration::GetTrendType1()
{
	return m_iTypeTrend1;
}

/**********************************************************************************************//**
 * Sets trend type 2
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::SetTrendType2(UINT type)
{
	m_iTypeTrend2=type;
	getModel()->getI2C()->WriteConfigByte(TREND_TYPE2_8, type);
}

/**********************************************************************************************//**
 * Gets trend type 2
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trend type 2.
 **************************************************************************************************/

UINT CConfiguration::GetTrendType2()
{
	return m_iTypeTrend2;
}

/**********************************************************************************************//**
 * Sets trend type 3
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CConfiguration::SetTrendType3(UINT type)
{
	m_iTypeTrend3=type;
	getModel()->getI2C()->WriteConfigByte(TREND_TYPE3_8, type);
}

/**********************************************************************************************//**
 * Gets trend type 3
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trend type 3.
 **************************************************************************************************/

UINT CConfiguration::GetTrendType3()
{
	return m_iTypeTrend3;
}

/**********************************************************************************************//**
 * Sets ppsv as delta peep value
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::setPpsvAsDeltaPEEPValue(BOOL bState)
{
	m_bPPSVasDeltaPEEPValue=bState;
	getModel()->getI2C()->WriteConfigByte(PPSV_AS_PEEPDELTA_8, bState);
}

/**********************************************************************************************//**
 * Query if this object is ppsv as delta peep value
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if ppsv as delta peep value, false if not.
 **************************************************************************************************/

BOOL CConfiguration::isPpsvAsDeltaPEEPValue()
{
	return m_bPPSVasDeltaPEEPValue;
}

/**********************************************************************************************//**
 * Sets leak compensation
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	leakComp	The leak component.
 **************************************************************************************************/

void CConfiguration::setLeakCompensation(eLeakCompensation leakComp)
{
	m_eLeakCompensation=leakComp;
	getModel()->getI2C()->WriteConfigByte(LEAKCOMPENSATIONOFF_8, (int)m_eLeakCompensation);

	getModel()->Send_MODE_OPTION2(true,true);
	getModel()->writeLEAKCOMPENSATIONToLog();
}

/**********************************************************************************************//**
 * Gets leak compensation
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The leak compensation.
 **************************************************************************************************/

eLeakCompensation CConfiguration::getLeakCompensation()
{
	return m_eLeakCompensation;
}

/**********************************************************************************************//**
 * Gets the next service date
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The next service date.
 **************************************************************************************************/

COleDateTime CConfiguration::getNextServiceDate()
{
	return m_dtNextServiceDate;
}

/**********************************************************************************************//**
 * Sets next service date
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iYear 	Zero-based index of the year.
 * \param	iMonth	Zero-based index of the month.
 * \param	iDate 	Zero-based index of the date.
 **************************************************************************************************/

void CConfiguration::setNextServiceDate(WORD iYear, WORD iMonth, WORD iDate)
{
	getModel()->getI2C()->WriteConfigWord(SERVICE_YEAR_16, iYear);
	getModel()->getI2C()->WriteConfigByte(SERVICE_MONTH_8, iMonth);
	getModel()->getI2C()->WriteConfigByte(SERVICE_DAY_8, iDate);
	m_dtNextServiceDate.SetDate(iYear, iMonth, iDate);
	getModel()->isMaintenanceNeeded();
}

/**********************************************************************************************//**
 * Graph set primary line diagramm
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::GraphSetPrimaryLineDiagramm(int value)
{
	m_iPrimaryLineDiagramm=value;
	getModel()->getI2C()->WriteConfigByte(CURLINEDIAGRAMM_8, value);
}

/**********************************************************************************************//**
 * Graph get primary line diagramm
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	An int.
 **************************************************************************************************/

int CConfiguration::GraphGetPrimaryLineDiagramm()
{
	return m_iPrimaryLineDiagramm;
}

/**********************************************************************************************//**
 * Gets system hw revision
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The system hw revision.
 **************************************************************************************************/

BYTE CConfiguration::getSystemHWrevision()
{
	return m_iSystemHWrevision;
}

/**********************************************************************************************//**
 * Gets development type
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The development type.
 **************************************************************************************************/

eDevType CConfiguration::getDevType()
{
	return m_eDevType;
}

/**********************************************************************************************//**
 * Gets flow sensor type
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The flow sensor type.
 **************************************************************************************************/

eFlowSensorType CConfiguration::getFlowSensorType()
{
	return m_eFlowSensorType;
}

/**********************************************************************************************//**
 * Gets av revision
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	av revision.
 **************************************************************************************************/

eAVRevision CConfiguration::getAVRevision()
{
	return m_eAVRevision;
}

/**********************************************************************************************//**
 * Gets hf module revision
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The hf module revision.
 **************************************************************************************************/

eHFModuleRevision CConfiguration::getHFModuleRevision()
{
	return m_eHFRevision;
}

/**********************************************************************************************//**
 * Gets main board version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The main board version.
 **************************************************************************************************/

BYTE CConfiguration::GetMainBoardVersion()
{
	return m_iMainBoard;
}

/**********************************************************************************************//**
 * Sets main board version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	vers	The vers.
 **************************************************************************************************/

void CConfiguration::setMainBoardVersion(UINT vers)
{
	m_iMainBoard=vers;
	getModel()->getI2C()->WriteConfigByte(HWCONF_MAINBOARD,vers);
}

/**********************************************************************************************//**
 * Sets temporary spo2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mod	The modifier.
 **************************************************************************************************/

void CConfiguration::setTempSPO2module(BYTE mod)
{
	m_bySPO2ModuleTemp=mod;
}

/**********************************************************************************************//**
 * Gets temporary spo2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The temporary spo2module.
 **************************************************************************************************/

BYTE CConfiguration::getTempSPO2module()
{
	return m_bySPO2ModuleTemp;
}

/**********************************************************************************************//**
 * Gets spo2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The spo2module.
 **************************************************************************************************/

BYTE CConfiguration::getSPO2module()
{
	return m_iSPO2module;
}

/**********************************************************************************************//**
 * Sets spo2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mod	   	The modifier.
 * \param	bReinit	True to reinitialize.
 **************************************************************************************************/

void CConfiguration::setSPO2module(BYTE mod, bool bReinit)
{
	m_bSpO2ConfigInProgress=true;

	bool bModuleChanged=false;
	if(m_iSPO2module!=mod)
		bModuleChanged=true;

	m_iSPO2module=mod;
	m_bySPO2ModuleTemp=mod;

	getModel()->getI2C()->WriteConfigByte(SPO2CONFIG,m_iSPO2module);

	getModel()->DeinitSPO2module(bReinit);

	if(m_iSPO2module!=SPO2MODULE_NONE)
	{
		if(getModel()->getAcuLink() != NULL)
		{
			if(getModel()->getPRICOThread())
			{
				if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange());		
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, getModel()->getDATAHANDLER()->getPRICO_SPO2highRange());
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange());
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, getModel()->getDATAHANDLER()->getPRICO_FIO2highRange());
				}
				else
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);
				}
			}
			else
			{
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);
			}
			
			if(GetAlarmlimitStateSPO2_SIQmin()==AL_OFF)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, getModel()->getALARMHANDLER()->getAlimitSPO2_SIQmin());

			if(GetAlarmlimitStateSPO2max()==AL_OFF)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, getModel()->getALARMHANDLER()->getAlimitSPO2max());

			if(GetAlarmlimitStateSPO2min()==AL_OFF)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, getModel()->getALARMHANDLER()->getAlimitSPO2min());

			if(GetAlarmlimitStateSPO2_PImin()==AL_OFF)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, getModel()->getALARMHANDLER()->getAlimitSPO2_PImin());

			if(GetAlarmlimitStatePulseRatemax()==AL_OFF)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, getModel()->getALARMHANDLER()->getAlimitPulseRatemax());

			if(GetAlarmlimitStatePulseRatemin()==AL_OFF)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, getModel()->getALARMHANDLER()->getAlimitPulseRatemin());
		}

		getModel()->initSPO2module(bReinit);

		if(bModuleChanged)
			getModel()->getALARMHANDLER()->ventModeChanged();
	}
	else
	{
		if(getModel()->getPRICOThread())
		{
			if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
			{
				getModel()->getDATAHANDLER()->setPRICOoff();
			}

			if(getModel()->getALARMHANDLER()->isPRICOAutoTurnedOff())
			{
				getModel()->getALARMHANDLER()->resetPRICOAutoTurnedOff();
			}
		}

		getModel()->DeinitSPO2module();
		if(getModel()->getALARMHANDLER()->isAlarmType(AT_SPO2))
		{
			getModel()->getALARMHANDLER()->deleteAlarmType(AT_SPO2);
		}

		if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2max->getAlarmState()!=AS_NONE)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2max);
		}
		if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2min->getAlarmState()!=AS_NONE)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2min);
		}

		if(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemax->getAlarmState()!=AS_NONE)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PulseRatemax);
		}
		if(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemin->getAlarmState()!=AS_NONE)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PulseRatemin);
		}

		if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_PImin->getAlarmState()!=AS_NONE)
		{
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2_PImin);
		}

		//NEWACULINK
		if (getModel()->getAcuLink() != NULL)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, ALINK_NOTVALID);
			getModel()->getAcuLink()->sendAllMeasurementData();
		}
	}
	m_bSpO2ConfigInProgress=false;
	
	if(!bReinit)
		getModel()->logSPO2module();
}

/**********************************************************************************************//**
 * Query if this object is spo2 configuration in progress
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if spo2 configuration in progress, false if not.
 **************************************************************************************************/

bool CConfiguration::isSpO2ConfigInProgress()
{
	return m_bSpO2ConfigInProgress;
}

/**********************************************************************************************//**
 * Sets temporary co2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mod	The modifier.
 **************************************************************************************************/

void CConfiguration::setTempCO2module(BYTE mod)
{
	m_byCO2ModuleTemp=mod;
}

/**********************************************************************************************//**
 * Gets temporary co2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The temporary co 2module.
 **************************************************************************************************/

BYTE CConfiguration::getTempCO2module()
{
	return m_byCO2ModuleTemp;
}

/**********************************************************************************************//**
 * Gets co2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The co2module.
 **************************************************************************************************/

BYTE CConfiguration::getCO2module()
{
	return m_iCO2module;
}

/**********************************************************************************************//**
 * Sets co2module
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	mod	The modifier.
 **************************************************************************************************/

void CConfiguration::setCO2module(BYTE mod)//CO2RKU
{
	if (m_iCO2module == mod)
	{
		return;
	}

	getModel()->setCO2inprogress(true);

	m_iCO2module = mod;
	m_byCO2ModuleTemp = m_iCO2module;

	getModel()->getI2C()->WriteConfigByte(CO2CONFIG, m_iCO2module);

	//DEBUGMSG(TRUE, (TEXT("DeinitCO2module\r\n"));
	getModel()->DeinitCO2module();

	Sleep(100);

	if (getModel()->getAcuLink() != NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_CO2, ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, ALINK_NOTVALID);

		getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2, ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_BAROPRESSURE_CO2, ALINK_NOTVALID);
		getModel()->getAcuLink()->sendAllMeasurementData();
	}


	if (m_iCO2module != CO2MODULE_NONE)
	{
		getModel()->getALARMHANDLER()->ventModeChanged();

		if (GetMainBoardVersion() >= MAINBOARD_30)
		{
			getModel()->initCO2module();

			if (getModel()->getAcuLink() != NULL)
			{
				getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_CO2, (int)GetCO2unit());
				if(GetAlarmlimitStateETCO2max())
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, AL_OFF);
				else
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, getModel()->getALARMHANDLER()->getAlimitETCO2max());
				
				if(GetAlarmlimitStateETCO2min())
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, AL_OFF);
				else
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, getModel()->getALARMHANDLER()->getAlimitETCO2min());

				if(GetAlarmlimitStateFICO2max())
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, AL_OFF);
				else
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, getModel()->getALARMHANDLER()->getAlimitETCO2max());
				
				if(GetAlarmlimitStateFICO2min())
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, AL_OFF);
				else
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, getModel()->getALARMHANDLER()->getAlimitETCO2min());

				if(getCO2module()==CO2MODULE_MICROPOD)
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2, ALINK_NOTVALID);
				}
				else
				{
					if (getModel()->isO2FlushActive())
						getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2, getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara());
					else
						getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2, getModel()->getDATAHANDLER()->PARADATA()->GetO2Para());
				}
				

				getModel()->getAcuLink()->setParaData(ALINK_SETT_BAROPRESSURE_CO2, getModel()->getCONFIG()->GetCO2BaroPressure());

			}
		}
		else //if(GetMainBoardVersion()<MAINBOARD_30)
		{
			if (GetPDMSprotocol() == ACL_SERIAL_ASCII
				|| GetPDMSprotocol() == ACL_SERIAL_WAVE
				|| GetPDMSprotocol() == ACL_SERIAL_IVOI
				|| GetPDMSprotocol() == ACL_ETHERNET_WAVE)
			{
				if (AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PDMS_ACTIVE);
			}
			else
			{
				getModel()->initCO2module();
			}
		}
	}

	if (m_iCO2module == CO2MODULE_NONE)
	{
		getModel()->setCO2inprogress(false);

		if (getModel()->getALARMHANDLER()->isAlarmType(AT_CO2))
			getModel()->getALARMHANDLER()->deleteAlarmType(AT_CO2);

		if (getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState() != AS_NONE)
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2max);

		if (getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState() != AS_NONE)
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2min);

		if (getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState() != AS_NONE)
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2max);

		if (getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState() != AS_NONE)
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2min);

		if (AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_CO2PUMPTIMER);
	}

	getModel()->setCO2inprogress(false);
	getModel()->logCO2module();
}

/**********************************************************************************************//**
 * Gets ethernet port
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The ethernet port.
 **************************************************************************************************/

UINT CConfiguration::GetEthernetPort()
{
	return m_iEthernetPort;
}

/**********************************************************************************************//**
 * Sets ethernet port
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	port	The port.
 **************************************************************************************************/

void CConfiguration::SetEthernetPort(UINT port)
{
	m_iEthernetPort=port;

	getModel()->getI2C()->WriteConfigWord(ETHERNETPORT_16,m_iEthernetPort);
}

/**********************************************************************************************//**
 * Gets pdms protocol
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The pdms protocol.
 **************************************************************************************************/

BYTE CConfiguration::GetPDMSprotocol()
{
	return m_iPDMSProtocol;
}

/**********************************************************************************************//**
 * Sets pdms protocol
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	prot	The prot.
 **************************************************************************************************/

void CConfiguration::SetPDMSprotocol(BYTE prot)
{
	m_iPDMSProtocol=prot;

	getModel()->getI2C()->WriteConfigByte(PDMSCONFIG,m_iPDMSProtocol);
}

/**********************************************************************************************//**
 * Gets acu link version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The acu link version.
 **************************************************************************************************/

eAcuLinkVersion CConfiguration::GetAcuLinkVersion()
{
	return m_eAcuLinkVersion;
}

/**********************************************************************************************//**
 * Sets acu link version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	vers	The vers.
 **************************************************************************************************/

void CConfiguration::SetAcuLinkVersion(eAcuLinkVersion vers)
{
	m_eAcuLinkVersion=vers;
}

/**********************************************************************************************//**
 * Gets vent range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The calculated vent range.
 **************************************************************************************************/

int CConfiguration::GetVentRange()
{
	return m_iVentRange;
}

/**********************************************************************************************//**
 * Sets vent range
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	range	The range.
 **************************************************************************************************/

void CConfiguration::SetVentRange(int range)
{
	if(NEONATAL==range)
		theApp.WriteLog(_T("### NEONATAL MODE ###"));
	else
		theApp.WriteLog(_T("### PEDIATRIC MODE ###"));

	bool bCheckSettings=false;
	if(m_iVentRange!=range)
	{
		bCheckSettings=true;
	}
	
	m_iVentRange=range;
	getModel()->getI2C()->WriteConfigByte(VENTRANGE_8, range);

	if(bCheckSettings)
	{
		//if(NEONATAL==range && CURVE_LINEAR==GetCurPressureRiseCtrl())//RISE_NEO
		//{
		//	getModel()->getCONFIG()->SetCurPressureRiseCtrl(CURVE_IFLOW);
		//}
		checkVentRangeSettings();

		getModel()->setVentilationRangeChanged(true);
	}
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);

	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VENTRANGE,range);

	if(getModel()->getCONFIG()->getCO2module()==CO2MODULE_MICROPOD && getModel()->getETCO2()!=NULL)
	{
		if(range==NEONATAL)
			getModel()->getETCO2()->set_PatientMode(2);
		else
			getModel()->getETCO2()->set_PatientMode(3);
		
	}

	/*if(getModel()->getCONFIG()->CurModeIsPresetMode())
		getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetPrevMode());
	else
		getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetCurMode());*/
}

/**********************************************************************************************//**
 * Check vent range settings
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

void CConfiguration::checkVentRangeSettings()
{
	//check max scaling of volume graph/loop
	int iUPPER_MAXSCALE_VOLUME=0;
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
	else
		iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

	if(GetMAXSCALE_VOLUME_GRAPH()>iUPPER_MAXSCALE_VOLUME)
	{
		SetMAXSCALE_VOLUME_GRAPH(iUPPER_MAXSCALE_VOLUME);
	}

	if(GetMAXSCALE_VOLUME_PVLOOP()>iUPPER_MAXSCALE_VOLUME)
	{
		SetMAXSCALE_VOLUME_PVLOOP(iUPPER_MAXSCALE_VOLUME);
	}

	if(GetMAXSCALE_VOLUME_VFLOOP()>iUPPER_MAXSCALE_VOLUME)
	{
		SetMAXSCALE_VOLUME_VFLOOP(iUPPER_MAXSCALE_VOLUME);
	}

	//check E-Flow EFLOW_CHECK
	WORD iLowerLimit=0;
	WORD iUpperLimit=0;
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
		iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
	}
	else
	{
		iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
		iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
	}

	WORD iValue=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER();
	if(iValue>iUpperLimit)
	{
		getModel()->getDATAHANDLER()->SetEFlowParadata_TRIGGER(iUpperLimit,false);
	}
	else if(iValue<iLowerLimit)
	{
		getModel()->getDATAHANDLER()->SetEFlowParadata_TRIGGER(iLowerLimit,false);
	}

	if(getModel()->getDATAHANDLER()->isFOTLicenseAvailable()==true)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_STOPANDRESET_FOT);
	}
}

/**********************************************************************************************//**
 * Determines if we can current mode is preset mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::CurModeIsPresetMode()
{
	eVentMode mode=VM_NONE;
	EnterCriticalSection(&csVentMode);
	mode=m_eCurMode;
	LeaveCriticalSection(&csVentMode);

	if(		mode == VM_PRE_IPPV
		||	mode == VM_PRE_PSV
		||	mode == VM_PRE_SIMV
		||	mode == VM_PRE_SIMVPSV
		||	mode == VM_PRE_SIPPV
		||	mode == VM_PRE_CPAP
		||	mode == VM_PRE_NCPAP
		||	mode == VM_PRE_DUOPAP
		||	mode == VM_PRE_HFO
		||	mode == VM_PRE_THERAPIE)
	{
		return true;
	}
	else
		return false;
}

/**********************************************************************************************//**
 * Sets current pressure rise control
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	form	The form.
 **************************************************************************************************/

void CConfiguration::SetCurPressureRiseCtrl(eCurveForm form)
{
	int iIFlow=0;
	int iRiseTime=0;

	switch(GetCurMode())
	{
	case VM_HFO:
	case VM_PRE_HFO:
		{
			
		}
		break;
	default:
		{
			
			m_eCurveForm=form;
			getModel()->getI2C()->WriteConfigByte(PRESSURERISECTRL_8, (int)form);
		}
		break;
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		iIFlow=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV();
		iRiseTime=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_IPPV();
	}
	else
	{
		iIFlow=getModel()->getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER();
		iRiseTime=getModel()->getDATAHANDLER()->PARADATA()->GetRisetimePara_TRIGGER();
	}
	
	switch(m_eCurveForm)
	{
	case CURVE_IFLOW:
		{
			if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,iIFlow/10);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,iIFlow);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_RISETIME,ALINK_NOTVALID);

			if(IsEFLOWequalILFOW())
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_EFLOWEQUIFLOW);
			}
		}
		break;
	case CURVE_LINEAR:
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,ALINK_NOTVALID);
			if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_RISETIME,iRiseTime/10);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_RISETIME,iRiseTime);

			//SetEFLOWequalILFOW(false);
		}
		break;
	case CURVE_AUTOFLOW:
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_RISETIME,ALINK_NOTVALID);

			//SetEFLOWequalILFOW(false);
		}
		break;
	}

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);
}

/**********************************************************************************************//**
 * Gets current pressure rise control
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The current pressure rise control.
 **************************************************************************************************/

eCurveForm CConfiguration::GetCurPressureRiseCtrl()
{
	 
	switch(GetCurMode())
	{
	case VM_HFO:
	case VM_PRE_HFO:
		{
			return CURVE_IFLOW;
		}
		break;
	default:
		{
			return m_eCurveForm;
		}
		break;
	}
	
}

/**********************************************************************************************//**
 * Gets current mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The current mode.
 **************************************************************************************************/

eVentMode CConfiguration::GetCurMode()//newSPI
{
	eVentMode mode=VM_NONE;
	EnterCriticalSection(&csVentMode);
	mode=m_eCurMode;
	LeaveCriticalSection(&csVentMode);

	return mode;
}

/**********************************************************************************************//**
 * Sets current mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetCurMode(eVentMode state)//newSPI
{
	if(		state != VM_PRE_IPPV
		&&	state != VM_PRE_PSV
		&&	state != VM_PRE_SIMV
		&&	state != VM_PRE_SIMVPSV
		&&	state != VM_PRE_SIPPV
		&&	state != VM_PRE_CPAP
		&&	state != VM_PRE_NCPAP
		&&	state != VM_PRE_DUOPAP
		&&	state != VM_PRE_HFO
		&&	state != VM_SERVICE
		&&	state != VM_PRE_THERAPIE)
	{
		getModel()->getI2C()->WriteConfigByte(CURVENTMODE_8, state);
	}

	eVentMode mode=VM_NONE;
	EnterCriticalSection(&csVentMode);
	mode=m_eCurMode;
	LeaveCriticalSection(&csVentMode);

	if(		mode != VM_PRE_IPPV
		&&	mode != VM_PRE_PSV
		&&	mode != VM_PRE_SIMV
		&&	mode != VM_PRE_SIMVPSV
		&&	mode != VM_PRE_SIPPV
		&&	mode != VM_PRE_CPAP
		&&	mode != VM_PRE_NCPAP
		&&	mode != VM_PRE_DUOPAP
		&&	mode != VM_PRE_HFO
		&&	mode != VM_SERVICE
		&&	mode != VM_PRE_THERAPIE)
	{
		SetPrevMode(mode);
	}

	EnterCriticalSection(&csVentMode);
	m_eCurMode = state;
	LeaveCriticalSection(&csVentMode);
}

/**********************************************************************************************//**
 * Gets the previous mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The previous mode.
 **************************************************************************************************/

eVentMode CConfiguration::GetPrevMode()//newSPI
{
	eVentMode mode=VM_NONE;
	EnterCriticalSection(&csVentMode);
	mode=m_ePrevMode;
	LeaveCriticalSection(&csVentMode);

	return mode;
}

/**********************************************************************************************//**
 * Sets previous mode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetPrevMode(eVentMode state)//newSPI
{
	EnterCriticalSection(&csVentMode);
	m_ePrevMode = state;
	LeaveCriticalSection(&csVentMode);
	
	getModel()->getI2C()->WriteConfigByte(PREVVENTMODE_8, state);
}

/**********************************************************************************************//**
 * Gets current a limit numeric
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The current a limit numeric.
 **************************************************************************************************/

eALimitNumeric CConfiguration::getCurALimitNumeric()
{
	return m_eLastALimitNumeric;
}

/**********************************************************************************************//**
 * Sets current a limit numeric
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	numeric	The numeric.
 **************************************************************************************************/

void CConfiguration::setCurALimitNumeric(eALimitNumeric numeric)
{
	m_eLastALimitNumeric=numeric;
	getModel()->getI2C()->WriteConfigByte(ALIMITLASTNUMERICSTATE_8,numeric);
}

/**********************************************************************************************//**
 * Gets the last selected sv setting btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last selected sv setting btns.
 **************************************************************************************************/

int CConfiguration::GetLastSelectedSVSettingBtns()
{
	return m_iSetupView_SettingBtn;
}

/**********************************************************************************************//**
 * Sets last selected sv setting btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	btns	The btns.
 **************************************************************************************************/

void CConfiguration::SetLastSelectedSVSettingBtns(int btns)
{
	m_iSetupView_SettingBtn=btns;
}

/**********************************************************************************************//**
 * Gets the last selected sv co2 btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last selected sv co2 btns.
 **************************************************************************************************/

int CConfiguration::GetLastSelectedSVCO2Btns()
{
	return m_iSetupView_CO2Btn;
}

/**********************************************************************************************//**
 * Sets last selected sv co2 btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	btns	The btns.
 **************************************************************************************************/

void CConfiguration::SetLastSelectedSVCO2Btns(int btns)
{
	m_iSetupView_CO2Btn=btns;
}

/**********************************************************************************************//**
 * Gets the last selected sv spo2 btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last selected sv spo2 btns.
 **************************************************************************************************/

int CConfiguration::GetLastSelectedSVSPO2Btns()
{
	return m_iSetupView_SPO2Btn;
}

/**********************************************************************************************//**
 * Sets last selected sv spo2 btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	btns	The btns.
 **************************************************************************************************/

void CConfiguration::SetLastSelectedSVSPO2Btns(int btns)
{
	m_iSetupView_SPO2Btn=btns;
}

/**********************************************************************************************//**
 * Gets the last selected alimit btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last selected alimit btns.
 **************************************************************************************************/

int CConfiguration::GetLastSelectedALBtns()
{
	return m_iALimit_Btn;
}

/**********************************************************************************************//**
 * Sets last selected alimit btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	btns	The btns.
 **************************************************************************************************/

void CConfiguration::SetLastSelectedALBtns(int btns)
{
	//DEBUGMSG(TRUE, (TEXT("SetLastSelectedALBtns() %d\r\n"),btns));
	m_iALimit_Btn=btns;
}

/**********************************************************************************************//**
 * Gets the last selected fot view btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last selected fot view btns.
 **************************************************************************************************/

int CConfiguration::getLastSelectedFOTViewBtns()
{
	return m_iFOTViewBtn;
}

/**********************************************************************************************//**
 * Sets last selected fot view btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	btns	The btns.
 **************************************************************************************************/

void CConfiguration::setLastSelectedFOTViewBtns(int btns)
{
	m_iFOTViewBtn=btns;
}

/**********************************************************************************************//**
 * Readin fot vent delay time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

void CConfiguration::readinFOTventDelaytime()
{
	CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState\\FOT"),true);

	//regLang.WriteDWORD(_T("FOTDELAY"), 15);
	m_iFOTventDelaytime=(WORD)regLang.ReadDWORD(_T("FOTDELAY"), VENTDELAYTIME);

	if(m_iFOTventDelaytime<VENTDELAYTIME_MIN || m_iFOTventDelaytime>VENTDELAYTIME_MAX)
		m_iFOTventDelaytime=VENTDELAYTIME;
}

/**********************************************************************************************//**
 * Gets fot vent delay time
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fot vent delaytime.
 **************************************************************************************************/

WORD CConfiguration::getFOTventDelaytime()
{
	CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState\\FOT"),true);
	m_iFOTventDelaytime=(WORD)regLang.ReadDWORD(_T("FOTDELAY"), VENTDELAYTIME);

	if(m_iFOTventDelaytime<VENTDELAYTIME_MIN || m_iFOTventDelaytime>VENTDELAYTIME_MAX)
		m_iFOTventDelaytime=VENTDELAYTIME;
	return m_iFOTventDelaytime;
}

/**********************************************************************************************//**
 * Gets the last selected para view btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last selected para view btns.
 **************************************************************************************************/

int CConfiguration::GetLastSelectedParaViewBtns()
{
	//return m_iParaView_Btn;
	switch(GetCurMode())
	{
	case VM_IPPV:
	case VM_PRE_IPPV:
		{
			return m_iParaViewBtn_IPPV;
		}
		break;
	case VM_PSV:
	case VM_PRE_PSV:
		{
			return m_iParaViewBtn_PSV;
		}
		break;
	case VM_SIMV: 
	case VM_PRE_SIMV:
		{
			return m_iParaViewBtn_SIMV;
		}
		break;
	case VM_SIMVPSV:
	case VM_PRE_SIMVPSV:
		{
			return m_iParaViewBtn_SIMVPSV;
		}
		break;
	case VM_SIPPV:
	case VM_PRE_SIPPV:
		{
			return m_iParaViewBtn_SIPPV;
		}
		break;
	case VM_CPAP:
	case VM_PRE_CPAP:
		{
			return m_iParaViewBtn_CPAP;
		}
		break;
	case VM_NCPAP:
	case VM_PRE_NCPAP:
		{
			return m_iParaViewBtn_NCPAP;
		}
		break;
	case VM_DUOPAP:
	case VM_PRE_DUOPAP:
		{
			return m_iParaViewBtn_DUOPAP;
		}
		break;
	case VM_HFO:
	case VM_PRE_HFO:
		{
			return m_iParaViewBtn_HFO;
		}
		break;
	case VM_THERAPIE:
	case VM_PRE_THERAPIE:
		{
			return m_iParaViewBtn_THERAPIE;
		}
		break;
	case VM_SERVICE:
	default:
		{
			return m_iParaViewBtn_IPPV;
		}
		break;
	}
}

/**********************************************************************************************//**
 * Sets last selected para view btns
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	btns	The btns.
 **************************************************************************************************/

void CConfiguration::SetLastSelectedParaViewBtns(int btns)
{
	switch(GetCurMode())
	{
	case VM_IPPV: 
	case VM_PRE_IPPV:
		{
			m_iParaViewBtn_IPPV=btns;
		}
		break;
	case VM_PSV:
	case VM_PRE_PSV:
		{
			m_iParaViewBtn_PSV=btns;
		}
		break;
	case VM_SIMV: 
	case VM_PRE_SIMV:
		{
			m_iParaViewBtn_SIMV=btns;
		}
		break;
	case VM_SIMVPSV: 
	case VM_PRE_SIMVPSV:
		{
			m_iParaViewBtn_SIMVPSV=btns;
		}
		break;
	case VM_SIPPV:
	case VM_PRE_SIPPV:
		{
			m_iParaViewBtn_SIPPV=btns;
		}
		break;
	case VM_CPAP:
	case VM_PRE_CPAP:
		{
			m_iParaViewBtn_CPAP=btns;
		}
		break;
	case VM_NCPAP:
	case VM_PRE_NCPAP:
		{
			m_iParaViewBtn_NCPAP=btns;
		}
		break;
	case VM_DUOPAP:
	case VM_PRE_DUOPAP:
		{
			m_iParaViewBtn_DUOPAP=btns;
		}
		break;
	case VM_HFO:
	case VM_PRE_HFO:
		{
			m_iParaViewBtn_HFO=btns;
		}
		break;
	case VM_THERAPIE:
	case VM_PRE_THERAPIE:
		{
			m_iParaViewBtn_THERAPIE=btns;
		}
		break;
	case VM_SERVICE:
	default:
		{

		}
		break;
	}
}

/**********************************************************************************************//**
 * Gets para data itime ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data itime ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataITime_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataITime_IPPV;
#else
	return m_iParaDataITime_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets para data itime ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataITime_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataITime_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ITIME_IPPV_16, iVal);
#else
	m_iParaDataITime_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ITIME_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data itime trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data itime trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataITime_TRIGGER()
{
	return m_iParaDataITime_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data itime trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataITime_TRIGGER(WORD iVal)
{
	m_iParaDataITime_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ITIME_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data itime nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data itime nmode.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataITime_NMODE()
{
	return m_iParaDataITime_NMODE;
}

/**********************************************************************************************//**
 * Sets para data itime nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataITime_NMODE(WORD iVal)
{
	m_iParaDataITime_NMODE=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ITIME_NMODE_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data etime ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data etime ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataETIME_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataETIME_IPPV;
#else
	return m_iParaDataETIME_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data etime ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataETIME_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataETIME_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ETIME_IPPV_16, iVal);
#else
	m_iParaDataETIME_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ETIME_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data etime trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data etime trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataETIME_TRIGGER()
{
	return m_iParaDataETIME_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data etime trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataETIME_TRIGGER(WORD iVal)
{
	m_iParaDataETIME_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ETIME_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data etime nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data etime nmode.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataETIME_NMODE()
{
	return m_iParaDataETIME_NMODE;
}

/**********************************************************************************************//**
 * Sets para data etime nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataETIME_NMODE(WORD iVal)
{
	m_iParaDataETIME_NMODE=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_ETIME_NMODE_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data bpm ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data bpm ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataBPM_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataBPM_IPPV;
#else
	return m_iParaDataBPM_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data bpm ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataBPM_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataBPM_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_BPM_IPPV_16, iVal);

	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,m_iParaDataBPM_IPPV);*/
#else
	m_iParaDataBPM_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_BPM_TRIGGER_16, iVal);

	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,m_iParaDataBPM_TRIGGER);*/
#endif
}

/**********************************************************************************************//**
 * Gets para data bpm trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data bpm trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataBPM_TRIGGER()
{
	return m_iParaDataBPM_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data bpm trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataBPM_TRIGGER(WORD iVal)
{
	m_iParaDataBPM_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_BPM_TRIGGER_16, iVal);

	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,m_iParaDataBPM_TRIGGER);*/
}

/**********************************************************************************************//**
 * Gets para data bpm nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data bpm nmode.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataBPM_NMODE()
{
	return m_iParaDataBPM_NMODE;
}

/**********************************************************************************************//**
 * Sets para data bpm nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataBPM_NMODE(WORD iVal)
{
	m_iParaDataBPM_NMODE=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_BPM_NMODE_16, iVal);

	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,m_iParaDataBPM_NMODE);*/
}

/**********************************************************************************************//**
 * Gets para data iflow ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data iflow ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataIFlow_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataIFlow_IPPV;
#else
	return m_iParaDataIFlow_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data iflow ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataIFlow_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataIFlow_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_IPPV_16, iVal);
#else
	m_iParaDataIFlow_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data iflow trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data iflow trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataIFlow_TRIGGER()
{
	return m_iParaDataIFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data iflow trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataIFlow_TRIGGER(WORD iVal)
{
	m_iParaDataIFlow_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf flow.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataHFFlow()
{
	return m_iParaDataHFFlow;
}

/**********************************************************************************************//**
 * Sets para data hf flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFFlow(WORD iVal)
{
	m_iParaDataHFFlow=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFFLOW_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data risetime ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data risetime ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataRisetime_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataRisetime_IPPV;
#else
	return m_iParaDataRisetime_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data risetime ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataRisetime_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataRisetime_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARARISETIME_IPPV_16, iVal);
#else
	m_iParaDataRisetime_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_RISETIME_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data risetime trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data risetime trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataRisetime_TRIGGER()
{
	return m_iParaDataRisetime_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data risetime trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataRisetime_TRIGGER(WORD iVal)
{
	m_iParaDataRisetime_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_RISETIME_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data therapie flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data therapie flow.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataTherapieFlow()
{
	return m_iParaDataTherapieFlow;
}

/**********************************************************************************************//**
 * Sets para data therapie flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataTherapieFlow(WORD iVal)
{
	m_iParaDataTherapieFlow=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_THERAPYFLOW_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data eflow ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data eflow ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataEFlow_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataEFlow_IPPV;
#else
	return m_iParaDataEFlow_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data eflow ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataEFlow_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataEFlow_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_IPPV_16, iVal);
#else
	m_iParaDataEFlow_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data eflow trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data eflow trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataEFlow_TRIGGER()
{
	return m_iParaDataEFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data eflow trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataEFlow_TRIGGER(WORD iVal)
{
	m_iParaDataEFlow_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data flow minimum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The calculated para data flow minimum.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataFlowMin()
{
	return m_iParaDataFlowMin;
}

/**********************************************************************************************//**
 * Sets para data flow minimum
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataFlowMin(WORD iVal)
{
	m_iParaDataFlowMin=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_FLOWMIN_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data peep ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data peep ippv.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPEEP_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataPEEP_IPPV;
#else
	return m_iParaDataPEEP_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data peep ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPEEP_IPPV(SHORT iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataPEEP_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PEEP_IPPV_16, iVal);
#else
	m_iParaDataPEEP_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PEEP_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data peep trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data peep trigger.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPEEP_TRIGGER()
{
	return m_iParaDataPEEP_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data peep trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPEEP_TRIGGER(SHORT iVal)
{
	m_iParaDataPEEP_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PEEP_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Sets para data cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataCPAP(SHORT iVal)
{
	m_iParaDataCpap=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_CPAP_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data cpap nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data cpap nmode.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataCPAP_NMODE()
{
	return m_iParaDataNmode;
}

/**********************************************************************************************//**
 * Sets para data cpap nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataCPAP_NMODE(SHORT iVal)
{
	m_iParaDataNmode=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_CPAP_NMODE_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data p insp trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data p insp trigger.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPInsp_TRIGGER()
{
	return m_iParaDataPInsp_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data p insp trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPInsp_TRIGGER(SHORT iVal)
{
	m_iParaDataPInsp_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PINSP_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data p insp ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data p insp ippv.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPInsp_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataPInsp_IPPV;
#else
	return m_iParaDataPInsp_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data p insp ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPInsp_IPPV(SHORT iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataPInsp_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PINSP_IPPV_16, iVal);
#else
	m_iParaDataPInsp_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PINSP_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data pmax vg trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data pmax vg trigger.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPmaxVolG_TRIGGER()
{
	return m_iParaDataPmaxVolG_TRIGGER;
}

/**********************************************************************************************//**
 * Sets para data pmax vg trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPmaxVolG_TRIGGER(SHORT iVal)
{
	m_iParaDataPmaxVolG_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data pmax vg ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data pmax vg ippv.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPmaxVolG_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataPmaxVolG_IPPV;
#else
	return m_iParaDataPmaxVolG_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data pmax vg ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPmaxVolG_IPPV(SHORT iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataPmaxVolG_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_IPPV_16, iVal);
#else
	m_iParaDataPmaxVolG_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data ppsv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data ppsv.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPpsv()
{
	return m_iParaDataPPSV;
}

/**********************************************************************************************//**
 * Sets para data ppsv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPpsv(SHORT iVal)
{
	m_iParaDataPPSV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PPSV_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data p manual hfo
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data p manual hfo.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPManual_HFO()
{
	return m_iParaDataPManual_HFO;
}

/**********************************************************************************************//**
 * Sets para data p manual hfo
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal			Zero-based index of the value.
 * \param	bSendAcuLink	True to send acu link.
 **************************************************************************************************/

void CConfiguration::SetParaDataPManual_HFO(SHORT iVal, bool bSendAcuLink)//PMAN1
{
	m_iParaDataPManual_HFO=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PMAN_HFO_16, iVal);
	if(bSendAcuLink && getModel()->getAcuLink()!=NULL)//PMAN1
		getModel()->getAcuLink()->setParaData(ALINK_SETT_PMANUAL,iVal);
}

//SHORT CConfiguration::GetParaDataPManual_CPAP()
//{
//	return m_iParaDataPManual_CPAP;
//}

/**********************************************************************************************//**
 * Sets para data p manual cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal			Zero-based index of the value.
 * \param	bSendAcuLink	True to send acu link.
 **************************************************************************************************/

void CConfiguration::SetParaDataPManual_CPAP(SHORT iVal, bool bSendAcuLink)//PMAN1
{
	m_iParaDataPManual_CPAP=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PMAN_CPAP_16, iVal);
	if(bSendAcuLink && getModel()->getAcuLink()!=NULL)//PMAN1
		getModel()->getAcuLink()->setParaData(ALINK_SETT_PMANUAL,iVal);
}

/**********************************************************************************************//**
 * Gets para data p manual nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data p manual nmode.
 **************************************************************************************************/

SHORT CConfiguration::GetParaDataPManual_NMODE()
{
	return m_iParaDataPManual_NMODE;
}

/**********************************************************************************************//**
 * Sets para data p manual nmode
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataPManual_NMODE(SHORT iVal)
{
	m_iParaDataPManual_NMODE=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_PMAN_NMODE_16, iVal);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_PDUO,iVal);
}

/**********************************************************************************************//**
 * Gets para data o2
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data o2.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataO2()
{
	return m_iParaDataO2;
}

/**********************************************************************************************//**
 * Sets para data o2
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataO2(BYTE iVal)
{
	m_iParaDataO2=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_O2_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data o2 flush
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data o2 flush.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataO2Flush()
{
	return m_iParaDataO2Flush;
}

/**********************************************************************************************//**
 * Sets para data o2 flush
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataO2Flush(BYTE iVal)
{
	m_iParaDataO2Flush=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_O2FLUSH_16, iVal);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_O2FLUSH,iVal);
}

/**********************************************************************************************//**
 * Gets o2 difference
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The o2 difference.
 **************************************************************************************************/

BYTE CConfiguration::GetO2Difference()
{
	return m_iO2Difference;
}

/**********************************************************************************************//**
 * Sets o2 difference
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetO2Difference(BYTE value)
{
	m_iO2Difference=value;
	getModel()->getI2C()->WriteConfigWord(PARA_O2DIFF_16, value);

}

/**********************************************************************************************//**
 * Gets peep ppsv difference
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The peep ppsv difference.
 **************************************************************************************************/

int CConfiguration::getPEEP_PpsvDifference()
{
	return m_iPPSVDifference;
}

/**********************************************************************************************//**
 * Sets peep ppsv difference
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::setPEEP_PpsvDifference(int value)
{
	if(m_iPPSVDifference!=value)
	{
		DEBUGMSG(TRUE, (TEXT("CConfiguration::SetPPSVDifference %d\r\n"),value));
		m_iPPSVDifference=value;
		getModel()->getI2C()->WriteConfigWord(PARA_PPSVDIFF_16, value);
	}
	

}

/**********************************************************************************************//**
 * Gets pmean difference
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The pmean difference.
 **************************************************************************************************/

int	CConfiguration::GetPmeanDifference()
{
	return m_iPmeanDifference;
}

/**********************************************************************************************//**
 * Sets pmean difference
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetPmeanDifference(int value)
{
	if(m_iPmeanDifference!=value)
	{
		DEBUGMSG(TRUE, (TEXT("CConfiguration::SetPmeanDifference %d\r\n"),value));
		m_iPmeanDifference=value;
		getModel()->getI2C()->WriteConfigWord(PARA_PMEANDIFF_16, value);
	}
}

/**********************************************************************************************//**
 * Gets para data vlimit trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data vlimit trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataVLimit_TRIGGER()
{
	return m_iParaDataVLimit_TRIGGER;
}

/**********************************************************************************************//**
 * Gets para data vlimit ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data vlimit ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataVLimit_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataVLimit_IPPV;
#else
	return m_iParaDataVLimit_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data vlimit trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataVLimit_TRIGGER(WORD iVal)
{
	m_iParaDataVLimit_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Sets para data vlimit ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataVLimit_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataVLimit_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_IPPV_16, iVal);
#else
	m_iParaDataVLimit_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data vlimit state trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetParaDataVLimitState_TRIGGER()
{
	return m_bParaDataVLimitOn_TRIGGER;
}

/**********************************************************************************************//**
 * Gets para data vlimit state ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetParaDataVLimitState_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_bParaDataVLimitOn_IPPV;
#else
	return m_bParaDataVLimitOn_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data vlimit state trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::SetParaDataVLimitState_TRIGGER(bool bState)
{
	m_bParaDataVLimitOn_TRIGGER=bState;
	
	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 0);
	}
}

/**********************************************************************************************//**
 * Sets para data vlimit state ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::SetParaDataVLimitState_IPPV(bool bState)
{
#ifdef BACKUP_VENTILATION
	m_bParaDataVLimitOn_IPPV=bState;

	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_IPPV_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_IPPV_8, 0);
	}
#else
	m_bParaDataVLimitOn_TRIGGER=bState;

	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 0);
	}
#endif
	
}

/**********************************************************************************************//**
 * Gets para data vg trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data vg trigger.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataVGarant_TRIGGER()
{
	return m_iParaDataVGarant_TRIGGER;
}

/**********************************************************************************************//**
 * Gets para data vg ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data vg ippv.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataVGarant_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_iParaDataVGarant_IPPV;
#else
	return m_iParaDataVGarant_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets para data vg trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataVGarant_TRIGGER(WORD iVal)
{
	m_iParaDataVGarant_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_TRIGGER_16, iVal);
}

/**********************************************************************************************//**
 * Sets para data vg ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataVGarant_IPPV(WORD iVal)
{
#ifdef BACKUP_VENTILATION
	m_iParaDataVGarant_IPPV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_IPPV_16, iVal);
#else
	m_iParaDataVGarant_TRIGGER=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_TRIGGER_16, iVal);
#endif
}

/**********************************************************************************************//**
 * Gets para data vg state trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetParaDataVGarantState_TRIGGER()
{
	return m_bParaDataVGarantOn_TRIGGER;
}

/**********************************************************************************************//**
 * Gets para data vg state ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetParaDataVGarantState_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_bParaDataVGarantOn_IPPV;
#else
	return m_bParaDataVGarantOn_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets para data vg state trigger
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::SetParaDataVGarantState_TRIGGER(bool bState)
{
	m_bParaDataVGarantOn_TRIGGER=bState;
	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 0);
	}
}

/**********************************************************************************************//**
 * Sets para data vg state ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::SetParaDataVGarantState_IPPV(bool bState)
{
#ifdef BACKUP_VENTILATION
	m_bParaDataVGarantOn_IPPV=bState;
	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_IPPV_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_IPPV_8, 0);
	}
#else
	m_bParaDataVGarantOn_TRIGGER=bState;
	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 0);
	}
#endif
}
//BYTE CConfiguration::GetParaDataTriggerCONV()
//{
//	return m_iParaDataTriggerCONV;
//}
//void CConfiguration::SetParaDataTriggerCONV(BYTE iVal)
//{
//	m_iParaDataTriggerCONV=iVal;
//	getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CONV_16, iVal);
//}
//
//BYTE CConfiguration::GetParaDataTriggerNMODE()
//{
//	return m_iParaDataTriggerNMODE;
//}
//void CConfiguration::SetParaDataTriggerNMODE(BYTE iVal)
//{
//	m_iParaDataTriggerNMODE=iVal;
//	getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_NMODE_16, iVal);
//}

/**********************************************************************************************//**
 * Gets para data trigger convert
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data trigger convert.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataTrigger_CONV()
{
	return m_iParaDataTrigger_CONV;
}

/**********************************************************************************************//**
 * Sets para data trigger convert
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataTrigger_CONV(BYTE iVal)
{
	m_iParaDataTrigger_CONV=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CONVENT_16, iVal);

	//DEBUGMSG(TRUE, (TEXT("SetParaDataTrigger_CONV %d\r\n"),(int)iVal));
}

/**********************************************************************************************//**
 * Gets para data trigger cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data trigger cpap.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataTrigger_CPAP()
{
	return m_iParaDataTrigger_CPAP;
}

/**********************************************************************************************//**
 * Sets para data trigger cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataTrigger_CPAP(BYTE iVal)
{
	m_iParaDataTrigger_CPAP=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CPAP_16, iVal);

	//DEBUGMSG(TRUE, (TEXT("SetParaDataTrigger_CPAP %d\r\n"),(int)iVal));
}

/**********************************************************************************************//**
 * Gets para data trigger ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data trigger ncpap.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataTrigger_NCPAP()
{
	return m_iParaDataTrigger_NCPAP;
}

/**********************************************************************************************//**
 * Sets para data trigger ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataTrigger_NCPAP(BYTE iVal)
{
	m_iParaDataTrigger_NCPAP=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_NCPAP_16, iVal);

	//DEBUGMSG(TRUE, (TEXT("SetParaDataTrigger_NCPAP %d\r\n"),(int)iVal));
}

/**********************************************************************************************//**
 * Gets para data trigger duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data trigger duopap.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataTrigger_DUOPAP()
{
	return m_iParaDataTrigger_DUOPAP;
}

/**********************************************************************************************//**
 * Sets para data trigger duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataTrigger_DUOPAP(BYTE iVal)
{
	m_iParaDataTrigger_DUOPAP=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_DUOPAP_16, iVal);

	//DEBUGMSG(TRUE, (TEXT("SetParaDataTrigger_DUOPAP %d\r\n"),(int)iVal));
}

/**********************************************************************************************//**
 * Gets para data backup
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data backup.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataBackup()
{
	return m_iParaDataBackup;
}

/**********************************************************************************************//**
 * Sets para data backup
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataBackup(BYTE iVal)
{
	m_iParaDataBackup=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_BACKUP_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf ampl
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf ampl.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataHFAmpl()
{
	return m_iParaDataHFAmpl;
}

/**********************************************************************************************//**
 * Sets para data hf ampl
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFAmpl(WORD iVal)
{
	m_iParaDataHFAmpl=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFAMPL_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf amplmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf amplmax.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataHFAmplmax()
{
	return m_iParaDataHFAmplmax;
}

/**********************************************************************************************//**
 * Sets para data hf amplmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFAmplmax(WORD iVal)
{
	m_iParaDataHFAmplmax=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFAMPL_VOLG_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf frequency
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf frequency.
 **************************************************************************************************/

BYTE CConfiguration::GetParaDataHFFreq()
{
	return m_iParaDataHFFreq;
}

/**********************************************************************************************//**
 * Sets para data hf frequency
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFFreq(BYTE iVal)
{
	m_iParaDataHFFreq=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFFREQ_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf pmean
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf pmean.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataHFPmean()
{
	return m_iParaDataHFPmean;
}

/**********************************************************************************************//**
 * Sets para data hf pmean
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFPmean(WORD iVal)
{
	m_iParaDataHFPmean=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_PMEAN_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data itime record
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data itime record.
 **************************************************************************************************/

WORD	CConfiguration::GetParaDataITimeRec()
{
	return m_iParaDataHFITimeRec;
}

/**********************************************************************************************//**
 * Sets para data itime record
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataITimeRec(WORD iVal)
{
	m_iParaDataHFITimeRec=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_ITIMEREC16, iVal);
}

/**********************************************************************************************//**
 * Gets para data frequency record
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data frequency record.
 **************************************************************************************************/

WORD	CConfiguration::GetParaDataFREQ_REC()
{
	return m_iParaDataHFFREQ_REC;
}

/**********************************************************************************************//**
 * Sets para data frequency record
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataFREQ_REC(WORD iVal)
{
	m_iParaDataHFFREQ_REC=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_FREQREC16, iVal);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable())
		{
			if(iVal==0)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,ALINK_OFF);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,iVal);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,ALINK_NOTVALID);
		}
		
	}

}

/**********************************************************************************************//**
 * Gets para data hfp mean record
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hfp mean record.
 **************************************************************************************************/

WORD	CConfiguration::GetParaDataHFPMeanRec()
{
	return m_iParaDataHFPMeanRec;
}

/**********************************************************************************************//**
 * Sets para data hfp mean record
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFPMeanRec(WORD iVal)
{
	m_iParaDataHFPMeanRec=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_PMEANREC16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf vgarant
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf vgarant.
 **************************************************************************************************/

WORD CConfiguration::GetParaDataHFVGarant()
{
	return m_iParaDataHFVGarant;
}

/**********************************************************************************************//**
 * Sets para data hf vgarant
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFVGarant(WORD iVal)
{
	m_iParaDataHFVGarant=iVal;
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_VGARANT_16, iVal);
}

/**********************************************************************************************//**
 * Gets para data hf vgarant state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::GetParaDataHFVGarantState()
{
	return m_bParaDataHFVGarantState;
}

/**********************************************************************************************//**
 * Sets para data hf vgarant state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFVGarantState(bool bState)
{
	m_bParaDataHFVGarantState=bState;
	if(bState)
	{
		getModel()->getI2C()->WriteConfigByte(PARA_HFO_VGARANTSTATE_8, 1);
	}
	else
	{
		getModel()->getI2C()->WriteConfigByte(PARA_HFO_VGARANTSTATE_8, 0);
	}
}

/**********************************************************************************************//**
 * Gets para data hf ie ratio
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The para data hf ie ratio.
 **************************************************************************************************/

eRatioIE CConfiguration::GetParaDataHFIERatio()
{
	return m_iParaDataHFIERatio;
}

/**********************************************************************************************//**
 * Sets para data hf ie ratio
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CConfiguration::SetParaDataHFIERatio(eRatioIE iVal)
{
	m_iParaDataHFIERatio=iVal;
	getModel()->getI2C()->WriteConfigByte(PARA_HFO_IERATIO_8, (int)iVal);
}

/**********************************************************************************************//**
 * Determines if we can deactivate vgarant state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::DeaktivateVGarantState()
{
	/*m_iParaDataVGarant_TRIGGER=0;
	m_iParaDataVGarant_IPPV=0;
	m_iParaDataHFVGarant=0;
	getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_IPPV_16, 0);
	getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_TRIGGER_16, 0);
	getModel()->getI2C()->WriteConfigWord(PARA_HFO_VGARANT_16, 0);*/

	m_bParaDataVGarantOn_TRIGGER=false;
	m_bParaDataVGarantOn_IPPV=false;
	m_bParaDataHFVGarantState=false;
	getModel()->getI2C()->WriteConfigByte(PARA_HFO_VGARANTSTATE_8, 0);
	getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 0);
	getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_IPPV_8, 0);

	return true;
}

/**********************************************************************************************//**
 * Determines if we can deactivate vlimit state
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CConfiguration::DeaktivateVLimitState()
{
	m_bParaDataVLimitOn_TRIGGER=false;
	m_bParaDataVLimitOn_IPPV=false;

	
	getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 0);
	getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_IPPV_8, 0);

	return true;
}

/**********************************************************************************************//**
 * Current Alarmlimits
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo 2max.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2max()
{
	return m_iAlarmlimitSPO2max;
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2max.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2max()
{
	return (eAlarmLimitState)m_iAlarmlimitStateSPO2max;
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2max(int value)
{
	m_iAlarmlimitSPO2max=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_16, value);
	
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateSPO2max()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2max(eAlarmLimitState state)
{
	m_iAlarmlimitStateSPO2max=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit spo2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo2min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2min()
{
	return m_iAlarmlimitSPO2min;
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2min()
{
	return (eAlarmLimitState)m_iAlarmlimitStateSPO2min;
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2min(int value)
{
	m_iAlarmlimitSPO2min=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_16, value);
	
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateSPO2min()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2min(eAlarmLimitState state)
{
	m_iAlarmlimitStateSPO2min=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pulseratemax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pulseratemax.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPulseRatemax()
{
	return m_iAlarmlimitPulseRatemax;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pulseratemax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pulse ratemax.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePulseRatemax()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePulseRatemax;
}

/**********************************************************************************************//**
 * Sets alarmlimit pulseratemax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPulseRatemax(int value)
{
	m_iAlarmlimitPulseRatemax=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_16, value);
	
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStatePulseRatemax()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state pulseratemax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePulseRatemax(eAlarmLimitState state)
{
	m_iAlarmlimitStatePulseRatemax=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pulseratemin
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pulse ratemin.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPulseRatemin()
{
	return m_iAlarmlimitPulseRatemin;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pulseratemin
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pulse ratemin.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePulseRatemin()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePulseRatemin;
}

/**********************************************************************************************//**
 * Sets alarmlimit pulseratemin
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPulseRatemin(int value)
{
	m_iAlarmlimitPulseRatemin=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_16, value);
	
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStatePulseRatemin()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state pulseratemin
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePulseRatemin(eAlarmLimitState state)
{
	m_iAlarmlimitStatePulseRatemin=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit spo2 pi min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo2 pi min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2_PImin()
{
	return m_iAlarmlimitSPO2_PImin;
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2 pi min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2 pi min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2_PImin()
{
	return (eAlarmLimitState)m_iAlarmlimitStateSPO2_PImin;
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2 pi min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2_PImin(int value)
{
	m_iAlarmlimitSPO2_PImin=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_16, value);
	
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateSPO2_PImin()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2 pi min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2_PImin(eAlarmLimitState state)
{
	m_iAlarmlimitStateSPO2_PImin=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_PIMIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit spo2 SIQ min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo2 SIQ min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2_SIQmin()
{
	return m_iAlarmlimitSPO2_SIQmin;
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2 SIQ min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2 SIQ min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2_SIQmin()
{
	return (eAlarmLimitState)m_iAlarmlimitStateSPO2_SIQmin;
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2 SIQ min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2_SIQmin(int value)
{
	m_iAlarmlimitSPO2_SIQmin=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_SIQMIN_16, value);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateSPO2_SIQmin()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2 SIQ min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2_SIQmin(eAlarmLimitState state)
{
	m_iAlarmlimitStateSPO2_SIQmin=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_SIQMIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit etco2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit etco2max.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitETCO2max()
{
	return m_iAlarmlimitETCO2max;
}

/**********************************************************************************************//**
 * Gets alarmlimit state etco2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state etco2max.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateETCO2max()
{
	return (eAlarmLimitState)m_iAlarmlimitStateETCO2max;
}

/**********************************************************************************************//**
 * Sets alarmlimit etco2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitETCO2max(int value)
{
	m_iAlarmlimitETCO2max=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_16, value);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateETCO2max()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state etco2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateETCO2max(eAlarmLimitState state)
{
	m_iAlarmlimitStateETCO2max=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit etco2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit etco2min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitETCO2min()
{
	return m_iAlarmlimitETCO2min;
}

/**********************************************************************************************//**
 * Gets alarmlimit state etco2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state etco2min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateETCO2min()
{
	return (eAlarmLimitState)m_iAlarmlimitStateETCO2min;
}

/**********************************************************************************************//**
 * Sets alarmlimit etco2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitETCO2min(int value)
{
	m_iAlarmlimitETCO2min=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_16, value);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state etco2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateETCO2min(eAlarmLimitState state)
{
	m_iAlarmlimitStateETCO2min=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit fico2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit fico2max.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitFICO2max()
{
	return m_iAlarmlimitFICO2max;
}

/**********************************************************************************************//**
 * Gets alarmlimit state fico2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state fico2max.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateFICO2max()
{
	return (eAlarmLimitState)m_iAlarmlimitStateFICO2max;
}

/**********************************************************************************************//**
 * Sets alarmlimit fico2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitFICO2max(int value)
{
	m_iAlarmlimitFICO2max=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_16, value);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state fico2max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateFICO2max(eAlarmLimitState state)
{
	m_iAlarmlimitStateFICO2max=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit fico2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit fico2min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitFICO2min()
{
	return m_iAlarmlimitFICO2min;
}

/**********************************************************************************************//**
 * Gets alarmlimit state fico2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state fico2min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateFICO2min()
{
	return (eAlarmLimitState)m_iAlarmlimitStateFICO2min;
}

/**********************************************************************************************//**
 * Sets alarmlimit fico2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitFICO2min(int value)
{
	m_iAlarmlimitFICO2min=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_16, value);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit()==AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, ALINK_OFF);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, value);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state fico2min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateFICO2min(eAlarmLimitState state)
{
	m_iAlarmlimitStateFICO2min=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit mv max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit mv max.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitMVmax()
{
	return m_iAlarmlimitMVmax;
}

/**********************************************************************************************//**
 * Gets alarmlimit state mv max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state mv max.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateMVmax()
{
	return (eAlarmLimitState)m_iAlarmlimitStateMVmax;
}

/**********************************************************************************************//**
 * Sets alarmlimit mv max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitMVmax(int value)
{
	m_iAlarmlimitMVmax=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMAX_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state mv max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateMVmax(eAlarmLimitState state)
{
	m_iAlarmlimitStateMVmax=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit mv min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit mv min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitMVmin()
{
	return m_iAlarmlimitMVmin;
}

/**********************************************************************************************//**
 * Gets alarmlimit state mv min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state mv min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateMVmin()
{
	return (eAlarmLimitState)m_iAlarmlimitStateMVmin;
}

/**********************************************************************************************//**
 * Sets alarmlimit mv min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitMVmin(int value)
{
	m_iAlarmlimitMVmin=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMIN_16, value);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,value);
		}
	}	
}

/**********************************************************************************************//**
 * Sets alarmlimit state mv min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateMVmin(eAlarmLimitState state)
{
	m_iAlarmlimitStateMVmin=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMIN_8, m_iAlarmlimitStateMVmin);
}

/**********************************************************************************************//**
 * Gets alarmlimit pip max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pip max.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPIPmax()
{
	return m_iAlarmlimitPIPmax;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pip max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pip max.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePIPmax()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePIPmax;
}

/**********************************************************************************************//**
 * Sets alarmlimit pip max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPIPmax(int value)
{
	m_iAlarmlimitPIPmax=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PIPMAX_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pip max
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePIPmax(eAlarmLimitState state)
{
	m_iAlarmlimitStatePIPmax=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PIPMAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pip min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pip min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPIPmin()
{
	return m_iAlarmlimitPIPmin;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pip min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pip min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePIPmin()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePIPmin;
}

/**********************************************************************************************//**
 * Sets alarmlimit pip min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPIPmin(int value)
{
	m_iAlarmlimitPIPmin=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PIPMIN_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pip min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePIPmin(eAlarmLimitState state)
{
	m_iAlarmlimitStatePIPmin=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PIPMIN_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit peep min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit peep min.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPEEPmin()
{
	return m_iAlarmlimitPEEPmin;
}

/**********************************************************************************************//**
 * Gets alarmlimit state peep min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state peep min.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePEEPmin()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePEEPmin;
}

/**********************************************************************************************//**
 * Sets alarmlimit peep min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPEEPmin(int value)
{
	m_iAlarmlimitPEEPmin=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state peep min
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePEEPmin(eAlarmLimitState state)
{
	m_iAlarmlimitStatePEEPmin=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit bpmmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit bpmmax.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitBPMmax()
{
	return m_iAlarmlimitBPMmax;
}

/**********************************************************************************************//**
 * Gets alarmlimit state bpmmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state bpmmax.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateBPMmax()
{
	return (eAlarmLimitState)m_iAlarmlimitStateBPMmax;
}

/**********************************************************************************************//**
 * Sets alarmlimit bpmmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitBPMmax(int value)
{
	m_iAlarmlimitBPMmax=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_BPMMAX_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()!=AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,value);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state bpmmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateBPMmax(eAlarmLimitState state)
{
	m_iAlarmlimitStateBPMmax=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_BPMMAX_8, state);
}

/**********************************************************************************************//**
 * Gets alimit leakmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alimit leakmax.
 **************************************************************************************************/

int CConfiguration::getAlimitLeakmax()
{
	return m_iAlimitLeakmax;
}

/**********************************************************************************************//**
 * Gets alarmlimit state leakmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state leakmax.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateLeakmax()
{
	return (eAlarmLimitState)m_iAlarmlimitStateLeakmax;
}

/**********************************************************************************************//**
 * Sets alarmlimit leakmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitLeakmax(int value)
{
	m_iAlimitLeakmax=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_LEAKMAX_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()!=AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,value);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state leakmax
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateLeakmax(eAlarmLimitState state)
{
	m_iAlarmlimitStateLeakmax=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_LEAKMAX_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit apnoe
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit apnoe.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitApnoe()
{
	if(GetCurMode() == VM_IPPV)//newSPI
	{
		return 0;
	}
	else
	{
		return m_iAlarmlimitApnoe;
	}
}

/**********************************************************************************************//**
 * Gets alarmlimit state apnoe
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state apnoe.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateApnoe()
{
	if(GetCurMode() == VM_IPPV)//newSPI
	{
		return AL_OFF;
	}
	else
	{
		return (eAlarmLimitState)m_iAlarmlimitStateApnoe;
	}
}

/**********************************************************************************************//**
 * Sets alimit apnoe
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::setAlimitApnoe(int value)
{
	if(GetCurMode() == VM_IPPV)//newSPI
	{
		if(value>0)
		{
			value=0;
		}
	}
	m_iAlarmlimitApnoe=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_APNOE_16, value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state apnoe
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateApnoe(eAlarmLimitState state)
{
	m_iAlarmlimitStateApnoe=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_APNOE_8, state);
}

/**********************************************************************************************//**
 * Current Alarmlimits HFO
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo 2max hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2maxHF()
{
	//return m_iAlarmlimitSPO2maxHF;
	return GetAlarmlimitSPO2max();
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2max hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2maxHF()
{
	//return (eAlarmLimitState)m_iAlarmlimitStateSPO2maxHF;
	return GetAlarmlimitStateSPO2max();
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2maxHF(int value)
{
	/*m_iAlarmlimitSPO2maxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_HF_16, value);*/
	SetAlarmlimitSPO2max(value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2maxHF(eAlarmLimitState state)
{
	/*m_iAlarmlimitStateSPO2maxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MAX_HF_8, state);*/
	SetAlarmlimitStateSPO2max(state);
}

/**********************************************************************************************//**
 * Gets alarmlimit spo2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo2min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2minHF()
{
	//return m_iAlarmlimitSPO2minHF;
	return GetAlarmlimitSPO2min();
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2minHF()
{
	//return (eAlarmLimitState)m_iAlarmlimitStateSPO2minHF;
	return GetAlarmlimitStateSPO2min();
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2minHF(int value)
{
	/*m_iAlarmlimitSPO2minHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_HF_16, value);*/
	SetAlarmlimitSPO2min(value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2minHF(eAlarmLimitState state)
{
	/*m_iAlarmlimitStateSPO2minHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MIN_HF_8, state);*/
	SetAlarmlimitStateSPO2min(state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pulseratemax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pulseratemax hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPulseRatemaxHF()
{
	//return m_iAlarmlimitPulseRatemaxHF;
	return GetAlarmlimitPulseRatemax();
}

/**********************************************************************************************//**
 * Gets alarmlimit state pulseratemax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pulseratemax hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePulseRatemaxHF()
{
	//return (eAlarmLimitState)m_iAlarmlimitStatePulseRatemaxHF;
	return GetAlarmlimitStatePulseRatemax();
}

/**********************************************************************************************//**
 * Sets alarmlimit pulseratemax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPulseRatemaxHF(int value)
{
	/*m_iAlarmlimitPulseRatemaxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_HF_16, value);*/
	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX,value);*/
	SetAlarmlimitPulseRatemax(value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pulseratemax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePulseRatemaxHF(eAlarmLimitState state)
{
	/*m_iAlarmlimitStatePulseRatemaxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMAX_HF_8, state);*/
	SetAlarmlimitStatePulseRatemax(state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pulseratemin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pulseratemin hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPulseRateminHF()
{
	//return m_iAlarmlimitPulseRateminHF;
	return GetAlarmlimitPulseRatemin();
}

/**********************************************************************************************//**
 * Gets alarmlimit state pulseratemin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pulseratemin hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePulseRateminHF()
{
	//return (eAlarmLimitState)m_iAlarmlimitStatePulseRateminHF;
	return GetAlarmlimitStatePulseRatemin();
}

/**********************************************************************************************//**
 * Sets alarmlimit pulseratemin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPulseRateminHF(int value)
{
	//m_iAlarmlimitPulseRateminHF=value;
	//getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_HF_16, value);
	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN,value);*/
	SetAlarmlimitPulseRatemin(value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pulseratemin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePulseRateminHF(eAlarmLimitState state)
{
	/*m_iAlarmlimitStatePulseRateminHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMIN_HF_8, state);*/
	SetAlarmlimitStatePulseRatemin(state);
}

/**********************************************************************************************//**
 * Gets alarmlimit spo2 pi min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo2 pi min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2_PIminHF()
{
	//return m_iAlarmlimitSPO2_PIminHF;
	return GetAlarmlimitSPO2_PImin();
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2 pi min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2 pi min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2_PIminHF()
{
	//return (eAlarmLimitState)m_iAlarmlimitStateSPO2_PIminHF;
	return GetAlarmlimitStateSPO2_PImin();
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2 pi min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2_PIminHF(int value)
{
	/*m_iAlarmlimitSPO2_PIminHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_HF_16, value);*/
	/*if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2_PIMIN,value);*/
	SetAlarmlimitSPO2_PImin(value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2 pi min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2_PIminHF(eAlarmLimitState state)
{
	/*m_iAlarmlimitStateSPO2_PIminHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_PIMIN_HF_8, state);*/
	SetAlarmlimitStateSPO2_PImin(state);
}

/**********************************************************************************************//**
 * Gets alarmlimit spo2 SIQ min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit spo2 SIQ min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitSPO2_SIQminHF()
{
	//return m_iAlarmlimitSPO2_SIQminHF;
	return GetAlarmlimitSPO2_SIQmin();
}

/**********************************************************************************************//**
 * Gets alarmlimit state spo2 SIQ min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state spo2 SIQ min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateSPO2_SIQminHF()
{
	//return (eAlarmLimitState)m_iAlarmlimitStateSPO2_SIQminHF;
	return GetAlarmlimitStateSPO2_SIQmin();
}

/**********************************************************************************************//**
 * Sets alarmlimit spo2 SIQ min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitSPO2_SIQminHF(int value)
{
	/*m_iAlarmlimitSPO2_SIQminHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_HF_16, value);*/
	SetAlarmlimitSPO2_SIQmin(value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state spo2 SIQ min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateSPO2_SIQminHF(eAlarmLimitState state)
{
	/*m_iAlarmlimitStateSPO2_SIQminHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_SIQMIN_HF_8, state);*/
	SetAlarmlimitStateSPO2_SIQmin(state);
}

/**********************************************************************************************//**
 * Gets alarmlimit etco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit etco2max hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitETCO2maxHF()
{
	return m_iAlarmlimitETCO2maxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state etco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state etco2max hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateETCO2maxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateETCO2maxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit etco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitETCO2maxHF(int value)
{
	m_iAlarmlimitETCO2maxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_HF_16, value);

	if(GetAlarmlimitStateETCO2maxHF()==AL_OFF)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, ALINK_OFF);
	else
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state etco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateETCO2maxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateETCO2maxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MAX_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit etco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit etco2min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitETCO2minHF()
{
	return m_iAlarmlimitETCO2minHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state etco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state etco2min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateETCO2minHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateETCO2minHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit etco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitETCO2minHF(int value)
{
	m_iAlarmlimitETCO2minHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_HF_16, value);

	if(GetAlarmlimitStateETCO2minHF()==AL_OFF)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, ALINK_OFF);
	else
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state etco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateETCO2minHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateETCO2minHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MIN_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit fico2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit fico2max hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitFICO2maxHF()
{
	return m_iAlarmlimitFICO2maxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state fico2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state fico2max hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateFICO2maxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateFICO2maxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit fico2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitFICO2maxHF(int value)
{
	m_iAlarmlimitFICO2maxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_HF_16, value);

	if(GetAlarmlimitStateFICO2maxHF()==AL_OFF)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, ALINK_OFF);
	else
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state fico2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateFICO2maxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateFICO2maxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MAX_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit fico2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit fico2min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitFICO2minHF()
{
	return m_iAlarmlimitFICO2minHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state fico2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state fico2min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateFICO2minHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateFICO2minHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit fico2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitFICO2minHF(int value)
{
	m_iAlarmlimitFICO2minHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_HF_16, value);

	if(GetAlarmlimitStateFICO2minHF()==AL_OFF)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, ALINK_OFF);
	else
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state fico2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateFICO2minHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateFICO2minHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MIN_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit map max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit map max hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitMAPmaxHF()
{
	return m_iAlarmlimitMAPmaxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state map max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state map max hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateMAPmaxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateMAPmaxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit map max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitMAPmaxHF(int value)
{
	m_iAlarmlimitMAPmaxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MAPMAX_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateMAPmaxHF()==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMAX,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMAX,getModel()->getALARMHANDLER()->getAlimitMAPmax());
		}
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state map max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateMAPmaxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateMAPmaxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MAPMAX_HF_8, state);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(state==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMAX,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMAX,getModel()->getALARMHANDLER()->getAlimitMAPmax());
		}
	}
}

/**********************************************************************************************//**
 * Gets alarmlimit map min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit map min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitMAPminHF()
{
	return m_iAlarmlimitMAPminHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state map min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state map min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateMAPminHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateMAPminHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit map min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitMAPminHF(int value)
{
	m_iAlarmlimitMAPminHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MAPMIN_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(GetAlarmlimitStateMAPminHF()==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMIN,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMIN,getModel()->getALARMHANDLER()->getAlimitMAPmin());
		}
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state map min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateMAPminHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateMAPminHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MAPMIN_HF_8, state);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(state==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMIN,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMIN,getModel()->getALARMHANDLER()->getAlimitMAPmin());
		}
	}

}

/**********************************************************************************************//**
 * Gets alarmlimit dco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit dco2max hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitDCO2maxHF()
{
	return m_iAlarmlimitDCO2maxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state dco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state dco2max hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateDCO2maxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateDCO2maxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit dco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitDCO2maxHF(int value)
{
	m_iAlarmlimitDCO2maxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_DCO2MAX_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_OFF);
		}
		else if(GetAlarmlimitStateDCO2maxHF()==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,getModel()->getALARMHANDLER()->getAlimitDCO2max());
		}
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state dco2max hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateDCO2maxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateDCO2maxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_DCO2MAX_HF_8, state);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_OFF);
		}
		else if(state==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,getModel()->getALARMHANDLER()->getAlimitDCO2max());
		}
	}
}

/**********************************************************************************************//**
 * Gets alarmlimit dco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit dco2min hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitDCO2minHF()
{
	return m_iAlarmlimitDCO2minHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state dco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state dco2min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateDCO2minHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateDCO2minHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit dco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitDCO2minHF(int value)
{
	m_iAlarmlimitDCO2minHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_DCO2MIN_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_OFF);
		}
		else if(GetAlarmlimitStateDCO2minHF()==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,getModel()->getALARMHANDLER()->getAlimitDCO2min());
		}
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state dco2min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateDCO2minHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateDCO2minHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_DCO2MIN_HF_8, state);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_OFF);
		}
		else if(state==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,getModel()->getALARMHANDLER()->getAlimitDCO2min());
		}
	}
	
}

/**********************************************************************************************//**
 * Gets alarmlimit mvmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit mvmax hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitMVmaxHF()
{
	return m_iAlarmlimitMVmaxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state mvmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state mvmax hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateMVmaxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateMVmaxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit mvmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitMVmaxHF(int value)
{
	m_iAlarmlimitMVmaxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMAX_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state mvmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateMVmaxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateMVmaxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMAX_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit mvmin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit mvmin hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitMVminHF()
{
	return m_iAlarmlimitMVminHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state mvmin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state mvmin hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateMVminHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateMVminHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit mvmin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitMVminHF(int value)
{
	m_iAlarmlimitMVminHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMIN_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()==AL_OFF)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,ALINK_OFF);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,value);
		}
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state mvmin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateMVminHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateMVminHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMIN_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pmax hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPmaxHF()
{
	return m_iAlarmlimitPmaxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pmax hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePmaxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePmaxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit pmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPmaxHF(int value)
{
	m_iAlarmlimitPmaxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePmaxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStatePmaxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pee pmin hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pee pmin hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPEEPminHF()
{
	return m_iAlarmlimitPEEPminHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state peep min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state peep min hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePEEPminHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePEEPminHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit peep min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPEEPminHF(int value)
{
	m_iAlarmlimitPEEPminHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state peep min hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePEEPminHF(eAlarmLimitState state)
{
	m_iAlarmlimitStatePEEPminHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit bpmmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit bpmmax hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitBPMmaxHF()
{
	return m_iAlarmlimitBPMmaxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state bpmmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state bpmmax hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateBPMmaxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateBPMmaxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit bpmmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitBPMmaxHF(int value)
{
	m_iAlarmlimitBPMmaxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_BPMMAX_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()!=AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,value);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state bpmmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateBPMmaxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateBPMmaxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_BPMMAX_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit leakmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit leakmax hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitLeakmaxHF()
{
	return m_iAlarmlimitLeakmaxHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state leakmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state leakmax hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateLeakmaxHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateLeakmaxHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit leakmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitLeakmaxHF(int value)
{
	m_iAlarmlimitLeakmaxHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_LEAKMAX_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()!=AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,value);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state leakmax hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateLeakmaxHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateLeakmaxHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_LEAKMAX_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit apnoe hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit apnoe hf.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitApnoeHF()
{
	return m_iAlarmlimitApnoeHF;
}

/**********************************************************************************************//**
 * Gets alarmlimit state apnoe hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state apnoe hf.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStateApnoeHF()
{
	return (eAlarmLimitState)m_iAlarmlimitStateApnoeHF;
}

/**********************************************************************************************//**
 * Sets alarmlimit apnoe hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitApnoeHF(int value)
{
	m_iAlarmlimitApnoeHF=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_APNOE_HF_16, value);
	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
			getModel()->getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,value);
		else
			getModel()->getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sets alarmlimit state apnoe hf
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStateApnoeHF(eAlarmLimitState state)
{
	m_iAlarmlimitStateApnoeHF=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_APNOE_HF_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pmax cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pmax cpap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPmaxCpap()
{
	return m_iAlarmlimitPmaxCpap;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pmax cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pmax cpap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePmaxCpap()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePmaxCpap;
}

/**********************************************************************************************//**
 * Sets alarmlimit pmax cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPmaxCpap(int value)
{
	m_iAlarmlimitPmaxCpap=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_CPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pmax cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePmaxCpap(eAlarmLimitState state)
{
	m_iAlarmlimitStatePmaxCpap=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_CPAP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit peep min cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit peep min cpap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPEEPminCpap()
{
	return m_iAlarmlimitPEEPminCpap;
}

/**********************************************************************************************//**
 * Gets alarmlimit state peep min cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state peep min cpap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePEEPminCpap()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePEEPminCpap;
}

/**********************************************************************************************//**
 * Sets alarmlimit peep min cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPEEPminCpap(int value)
{
	m_iAlarmlimitPEEPminCpap=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_CPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state peep min cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePEEPminCpap(eAlarmLimitState state)
{
	m_iAlarmlimitStatePEEPminCpap=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_CPAP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pipmax duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pipmax duopap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPIPmaxDUOPAP()
{
	return m_iAlarmlimitPIPmaxDUOPAP;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pipmax duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pipmax duopap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePIPmaxDUOPAP()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePIPmaxDUOPAP;
}

/**********************************************************************************************//**
 * Sets alarmlimit pipmax duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPIPmaxDUOPAP(int value)
{
	m_iAlarmlimitPIPmaxDUOPAP=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_DUOPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pmax duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePIPmaxDUOPAP(eAlarmLimitState state)
{
	m_iAlarmlimitStatePIPmaxDUOPAP=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_DUOPAP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pipmin duopap
 *
 * \author	Rainer K�hner
 * \date	05.03.2018
 *
 * \return	The alarmlimit pipmin duopap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPIPminDUOPAP()
{
	return m_iAlarmlimitPIPminDUOPAP;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pipmin duopap
 *
 * \author	Rainer K�hner
 * \date	05.03.2018
 *
 * \return	The alarmlimit state pipmin duopap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePIPminDUOPAP()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePIPminDUOPAP;
}

/**********************************************************************************************//**
 * Sets alarmlimit pipmin duopap
 *
 * \author	Rainer K�hner
 * \date	05.03.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPIPminDUOPAP(int value)
{
	m_iAlarmlimitPIPminDUOPAP=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMIN_DUOPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pipmin duopap
 *
 * \author	Rainer K�hner
 * \date	05.03.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePIPminDUOPAP(eAlarmLimitState state)
{
	m_iAlarmlimitStatePIPminDUOPAP=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMIN_DUOPAP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit pmax ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit pmax ncpap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPmaxNCPAP()
{
	return m_iAlarmlimitPIPmaxNCPAP;
}

/**********************************************************************************************//**
 * Gets alarmlimit state pmax ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state pmax ncpap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePmaxNCPAP()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePmaxNCPAP;
}

/**********************************************************************************************//**
 * Sets alarmlimit pmax ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPmaxNCPAP(int value)
{
	m_iAlarmlimitPIPmaxNCPAP=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_NCPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state pmax ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePmaxNCPAP(eAlarmLimitState state)
{
	m_iAlarmlimitStatePmaxNCPAP=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_NCPAP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit peep min duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit peep min duopap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPEEPminDUOPAP()
{
	return m_iAlarmlimitPEEPminDUOPAP;
}

/**********************************************************************************************//**
 * Gets alarmlimit state peep min duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state peep min duopap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePEEPminDUOPAP()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePEEPminDUOPAP;
}

/**********************************************************************************************//**
 * Sets alarmlimit peep min duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPEEPminDUOPAP(int value)
{
	m_iAlarmlimitPEEPminDUOPAP=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_DUOPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state peep min duopap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePEEPminDUOPAP(eAlarmLimitState state)
{
	m_iAlarmlimitStatePEEPminDUOPAP=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_DUOPAP_8, state);
}

/**********************************************************************************************//**
 * Gets alarmlimit peep min ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit peep min ncpap.
 **************************************************************************************************/

int CConfiguration::GetAlarmlimitPEEPminNCPAP()
{
	return m_iAlarmlimitPEEPminNCPAP;
}

/**********************************************************************************************//**
 * Gets alarmlimit state peep min ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The alarmlimit state peep min ncpap.
 **************************************************************************************************/

eAlarmLimitState CConfiguration::GetAlarmlimitStatePEEPminNCPAP()
{
	return (eAlarmLimitState)m_iAlarmlimitStatePEEPminNCPAP;
}

/**********************************************************************************************//**
 * Sets alarmlimit peep min ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitPEEPminNCPAP(int value)
{
	m_iAlarmlimitPEEPminNCPAP=value;
	getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_NCPAP_16, value);
	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,value);
}

/**********************************************************************************************//**
 * Sets alarmlimit state peep min ncpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CConfiguration::SetAlarmlimitStatePEEPminNCPAP(eAlarmLimitState state)
{
	m_iAlarmlimitStatePEEPminNCPAP=(int)state;
	getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_NCPAP_8, state);
}

/**********************************************************************************************//**
 * Sets last written trend file
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type 	The type.
 * \param	count	Number of.
 **************************************************************************************************/

void CConfiguration::SetLastWrittenTrendFile(UINT type, BYTE count)
{
	switch(type)
	{
	case TREND_PINSP:
		{
			//DEBUGMSG(TRUE, (TEXT("CConfiguration::SetLastWrittenTrendFile %d\r\n"),(int)count));
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_PINSP_8, count);
		}
		break;
	case TREND_PMEAN:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_PMEAN_8, count);
		}
		break;
	case TREND_FIO2:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_FIO2_8, count);
		}
		break;
	case TREND_VTE:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_VTE_8, count);
		}
		break;
	case TREND_COMPLIANCE:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_COMPLIANCE_8, count);
		}
		break;
	case TREND_CO2HFO:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_CO2HFO_8, count);
		}
		break;
	case TREND_MV:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_MV_8, count);
		}
		break;
	case TREND_HFAMP:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_HFAMP_8, count);
		}
		break;
	case TREND_RSBI:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_RSBI_8, count);
		}
		break;
	case TREND_SHAREMVMAND:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_SHAREMVMAND_8, count);
		}
		break;
	case TREND_RESISTANCE:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_RESISTANCE_8, count);
		}
		break;
	case TREND_LEAK:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_LEAK_8, count);
		}
		break;
	case TREND_SPO2:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_SPO2_8, count);
		}
		break;
	case TREND_SPO2PR:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_SPO2PR_8, count);
		}
		break;
	case TREND_SPO2PI:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_SPO2PI_8, count);
		}
		break;
	case TREND_ETCO2:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_ETCO2_8, count);
		}
		break;
	case TREND_FREQUENCY:
		{
			getModel()->getI2C()->WriteConfigByte(TRENDFILE_FREQUENCY_8, count);
		}
		break;
	}
}

/**********************************************************************************************//**
 * Gets the last written trend file
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	type	The type.
 *
 * \return	The last written trend file.
 **************************************************************************************************/

BYTE CConfiguration::GetLastWrittenTrendFile(UINT type)
{
	BYTE byResult=0;

	switch(type)
	{
	case TREND_PINSP:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_PINSP_8);
			//DEBUGMSG(TRUE, (TEXT("CConfiguration::GetLastWrittenTrendFile %d\r\n"),(int)byResult));
		}
		break;
	case TREND_PMEAN:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_PMEAN_8);
		}
		break;
	case TREND_FIO2:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_FIO2_8);
		}
		break;
	case TREND_VTE:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_VTE_8);
		}
		break;
	case TREND_COMPLIANCE:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_COMPLIANCE_8);
		}
		break;
	case TREND_CO2HFO:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_CO2HFO_8);
		}
		break;
	case TREND_MV:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_MV_8);
		}
		break;
	case TREND_HFAMP:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_HFAMP_8);
		}
		break;
	case TREND_RSBI:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_RSBI_8);
		}
		break;
	case TREND_SHAREMVMAND:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_SHAREMVMAND_8);
		}
		break;
	case TREND_RESISTANCE:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_RESISTANCE_8);
		}
		break;
	case TREND_LEAK:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_LEAK_8);
		}
		break;
	case TREND_SPO2:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_SPO2_8);
		}
		break;
	case TREND_SPO2PR:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_SPO2PR_8);
		}
		break;
	case TREND_SPO2PI:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_SPO2PI_8);
		}
		break;
	case TREND_ETCO2:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_ETCO2_8);
		}
		break;
	case TREND_FREQUENCY:
		{
			byResult=getModel()->getI2C()->ReadConfigByte(TRENDFILE_FREQUENCY_8);
		}
		break;
	}

	if(byResult<1 || byResult>MAXTRENDFILES)
		byResult=0;

	return byResult;
}

/**********************************************************************************************//**
 * Gets patient remark
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The patient remark.
 **************************************************************************************************/

CStringW CConfiguration::GetPatientRemark()
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	return regPat.ReadString(_T("Remark"), _T(""));
}

/**********************************************************************************************//**
 * Gets patient name
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The patient name.
 **************************************************************************************************/

CStringW CConfiguration::GetPatientName()
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	return regPat.ReadString(_T("Name"), _T(""));
}

/**********************************************************************************************//**
 * Gets patient firstname
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The patient firstname.
 **************************************************************************************************/

CStringW CConfiguration::GetPatientFirstname()
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	return regPat.ReadString(_T("Firtsname"), _T(""));
}

/**********************************************************************************************//**
 * Gets patient identifier
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The patient identifier.
 **************************************************************************************************/

int CConfiguration::GetPatientID()
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	return regPat.ReadDWORD(_T("PersID"), 0);
	//return regPat.ReadString(_T("PersID"), _T(""));
}

/**********************************************************************************************//**
 * Gets patient weight
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The patient weight.
 **************************************************************************************************/

int CConfiguration::GetPatientWeight()
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	return regPat.ReadDWORD(_T("Weight"), 0);
}

/**********************************************************************************************//**
 * Gets patient birthday
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The patient birthday.
 **************************************************************************************************/

CStringW CConfiguration::GetPatientBirthday()
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	
	return regPat.ReadString(_T("DateOfBirth"), _T(""));
}

/**********************************************************************************************//**
 * Sets patient data
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	name	 	The name.
 * \param	firstName	The person's first name.
 * \param	persID   	Identifier for the pers.
 * \param	weight   	The weight.
 * \param	szTime   	The time.
 * \param	szRemark 	The remark.
 **************************************************************************************************/

void CConfiguration::SetPatientData(CStringW name, CStringW firstName, int persID, int weight, CStringW  szTime, CStringW  szRemark)
{
	CTlsRegistry regPat(_T("HKCU\\Software\\FabianHFO\\PatientData"),true);
	regPat.WriteString(_T("Name"), name);
	regPat.WriteString(_T("Firtsname"), firstName);
	regPat.WriteDWORD(_T("PersID"), persID);
	regPat.WriteDWORD(_T("Weight"), weight);
	regPat.WriteString(_T("DateOfBirth"), szTime);
	regPat.WriteString(_T("Remark"), szRemark);
	regPat.FlushHKCU();

	/*if(name!=_T(""))
		getModel()->getDATAHANDLER()->setPatientDataAvailable(true);
	else */
	if(persID!=0)
		getModel()->getDATAHANDLER()->setPatientDataAvailable(true);
	else
		getModel()->getDATAHANDLER()->setPatientDataAvailable(false);

	if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_PERSID,persID);
	}
}

/**********************************************************************************************//**
 * Gets the last calibration oxy 21
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last calibration oxy 21.
 **************************************************************************************************/

CStringW CConfiguration::GetLastCalOxy21()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	return regWorkState.ReadString(_T("LastCalOxy21"), _T(""));
}

/**********************************************************************************************//**
 * Sets last calibration oxy 21
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szDateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetLastCalOxy21(CStringW szDateTime)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteString(_T("LastCalOxy21"), szDateTime);

	regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Gets the last calibration oxy 100
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last calibration oxy 100.
 **************************************************************************************************/

CStringW CConfiguration::GetLastCalOxy100()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	return regWorkState.ReadString(_T("LastCalOxy100"), _T(""));
}

/**********************************************************************************************//**
 * Sets last calibration oxy 100
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szDateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetLastCalOxy100(CStringW szDateTime)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteString(_T("LastCalOxy100"), szDateTime);

	regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Gets the last calibration flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last calibration flow.
 **************************************************************************************************/

CStringW CConfiguration::GetLastCalFlow()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	return regWorkState.ReadString(_T("LastCalFlow"), _T(""));
}

/**********************************************************************************************//**
 * Sets last calibration flow
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szDateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetLastCalFlow(CStringW szDateTime)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteString(_T("LastCalFlow"), szDateTime);

	regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Gets the last calibration 0mbar
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last calibration 0mbar.
 **************************************************************************************************/

CStringW CConfiguration::GetLastCal0mbar()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	return regWorkState.ReadString(_T("LastCal0mbar"), _T(""));
}

/**********************************************************************************************//**
 * Sets last calibration 0mbar
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szDateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetLastCal0mbar(CStringW szDateTime)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteString(_T("LastCal0mbar"), szDateTime);

	regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Gets the last calibration 60mbar
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last calibration 60mbar.
 **************************************************************************************************/

CStringW CConfiguration::GetLastCal60mbar()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	return regWorkState.ReadString(_T("LastCal60mbar"), _T(""));
}

/**********************************************************************************************//**
 * Sets last calibration 60mbar
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	szDateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetLastCal60mbar(CStringW szDateTime)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteString(_T("LastCal60mbar"), szDateTime);

	regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Gets operating minutes device
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The operating minutes device.
 **************************************************************************************************/

DWORD CConfiguration::getOperatingMinutesDevice()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	DWORD iMin=regWorkState.ReadDWORD(_T("OpMinutesDevice"), 0);

	return iMin;
}

/**********************************************************************************************//**
 * Sets operating minutes device
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	min		 	The minimum.
 * \param	bFlushReg	True to flush register.
 **************************************************************************************************/

void CConfiguration::setOperatingMinutesDevice(DWORD min, bool bFlushReg)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteDWORD(_T("OpMinutesDevice"), min);
	if(bFlushReg)
		regWorkState.FlushHKCU();

}

/**********************************************************************************************//**
 * Gets operating minutes battery
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The operating minutes battery.
 **************************************************************************************************/

DWORD CConfiguration::getOperatingMinutesBattery()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	DWORD iMin=regWorkState.ReadDWORD(_T("OpMinutesBattery"), 0);

	return iMin;
}

/**********************************************************************************************//**
 * Sets operating minutes battery
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	min		 	The minimum.
 * \param	bFlushReg	True to flush register.
 **************************************************************************************************/

void CConfiguration::setOperatingMinutesBattery(DWORD min, bool bFlushReg)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteDWORD(_T("OpMinutesBattery"), min);
	if(bFlushReg)
		regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Gets operating minutes hfo
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The operating minutes hfo.
 **************************************************************************************************/

DWORD CConfiguration::getOperatingMinutesHFO()
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	DWORD iMin=regWorkState.ReadDWORD(_T("OpMinutesHFO"), 0);

	return iMin;
}

/**********************************************************************************************//**
 * Sets operating minutes hfo
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	min		 	The minimum.
 * \param	bFlushReg	True to flush register.
 **************************************************************************************************/

void CConfiguration::setOperatingMinutesHFO(DWORD min, bool bFlushReg)
{
	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	regWorkState.WriteDWORD(_T("OpMinutesHFO"), min);
	if(bFlushReg)
		regWorkState.FlushHKCU();
}

/**********************************************************************************************//**
 * Serialize file
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param [in,out]	ar	The archive.
 **************************************************************************************************/

void CConfiguration::SerializeFile(CArchive& ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar<<m_iConfigVersion;
		ar<<m_iEthernetPort;
		ar<<m_iPDMSProtocol;
		ar<<m_szLanguageID;
		ar<<m_iVentRange;
		ar<<m_bGraphAutoScale;
		ar<<m_bTrendAutoScale;
		ar<<m_bFilledGraph;
		ar<<m_bFilledTrend;
		//ar<<m_bAskPatDataStartup;
		ar<<m_bUseTveBTB;
		//ar<<m_bVolumeTrigger;
		bool bDummy=false;//dummy for old value m_bVolumeTrigger
		ar<<bDummy;
		ar<<(int)m_eIERelationMode;
		ar<<(int)m_eRefFlowSensor;
		ar<<(int)m_eTubeSet;
		ar<<(int)m_ePressureUnit;
		ar<<m_bPlayVlimitSound;
		ar<<m_bPlayBackupSound;
		ar<<m_byFlushTime;
		ar<<m_iManBreathTime;
		ar<<m_iNebulizerTime;
		ar<<m_iPminAlarmDelay;
		ar<<m_iPatientAlarmDelay;
		//ar<<m_iBreathVolGaranty;
		//ar<<m_bPlayTriggerSound;
		ar<<m_bHFOManBreathEnabled;
		ar<<m_iPercentAbortCriterionPSV;
		ar<<(int)m_eAutoOxyCal;
		ar<<(int)m_eAutoScreenlocktime;
		ar<<m_bAutoScreenlockActive;
		ar<<(int)m_eCO2unit;
		//ar<<m_bCO2ModuleEnabled;
		ar<<m_iCO2BaroPressure;
		ar<<m_iCO2TimePumpAutoOn;
		ar<<m_bServicePasswordDisabled;
		ar<<m_bNursecallActive;
		ar<<m_bEnableAutolimit;
		ar<<m_bEnableFactory;
		ar<<(int)m_eAlarmVolume;
		ar<<m_iTypeTrend1;
		ar<<m_iTypeTrend2;
		ar<<m_iTypeTrend3;
		ar<<m_iParaDataITime_IPPV;
		ar<<m_iParaDataITime_NMODE;
		ar<<m_iParaDataETIME_IPPV;
		ar<<m_iParaDataETIME_NMODE;
		ar<<m_iParaDataBPM_IPPV;
		ar<<m_iParaDataBPM_NMODE;
		ar<<m_iParaDataRisetime_IPPV;
		ar<<m_iParaDataIFlow_IPPV;
		ar<<m_iParaDataHFFlow;
		ar<<m_iParaDataEFlow_IPPV;
		ar<<m_iParaDataTherapieFlow;
		ar<<m_iParaDataFlowMin;
		ar<<m_iParaDataPEEP_IPPV;
		ar<<m_iParaDataCpap;
		ar<<m_iParaDataNmode;
		ar<<m_iParaDataPInsp_IPPV;
		ar<<m_iParaDataPmaxVolG_IPPV;
		ar<<m_iParaDataPPSV;
		ar<<m_iParaDataPManual_HFO;
		ar<<m_iParaDataPManual_NMODE;
		ar<<m_iParaDataO2;
		ar<<m_iParaDataO2Flush;
		ar<<m_iO2Difference;
		ar<<m_iPmeanDifference;
		ar<<m_iParaDataVLimit_IPPV;
		ar<<m_bParaDataVLimitOn_IPPV;
		ar<<m_iParaDataVGarant_IPPV;
		ar<<m_bParaDataVGarantOn_IPPV;
		ar<<m_iParaDataTrigger_CONV;
		ar<<m_iParaDataBackup;
		ar<<m_iParaDataHFAmpl;
		ar<<m_iParaDataHFAmplmax;
		ar<<m_iParaDataHFFreq;
		ar<<m_iParaDataHFPmean;
		ar<<m_iParaDataHFVGarant;
		ar<<m_bParaDataHFVGarantState;
		ar<<(int)m_iParaDataHFIERatio;
		ar<<m_iParaDataHFITimeRec;
		ar<<m_iParaDataHFFREQ_REC;
		ar<<m_iParaDataHFPMeanRec;
		ar<<m_iAlarmlimitETCO2max;
		ar<<m_iAlarmlimitStateETCO2max;
		ar<<m_iAlarmlimitETCO2min;
		ar<<m_iAlarmlimitStateETCO2min;
		ar<<m_iAlarmlimitMVmax;
		ar<<m_iAlarmlimitStateMVmax;
		ar<<m_iAlarmlimitMVmin;
		ar<<m_iAlarmlimitStateMVmin;
		ar<<m_iAlarmlimitPIPmax;
		ar<<m_iAlarmlimitStatePIPmax;
		ar<<m_iAlarmlimitPEEPmin;
		ar<<m_iAlarmlimitStatePEEPmin;
		ar<<m_iAlarmlimitBPMmax;
		ar<<m_iAlarmlimitStateBPMmax;
		ar<<m_iAlimitLeakmax;
		ar<<m_iAlarmlimitStateLeakmax;
		ar<<m_iAlarmlimitApnoe;
		ar<<m_iAlarmlimitStateApnoe;
		ar<<m_iAlarmlimitETCO2maxHF;
		ar<<m_iAlarmlimitStateETCO2maxHF;
		ar<<m_iAlarmlimitETCO2minHF;
		ar<<m_iAlarmlimitStateETCO2minHF;
		ar<<m_iAlarmlimitDCO2maxHF;
		ar<<m_iAlarmlimitStateDCO2maxHF;
		ar<<m_iAlarmlimitDCO2minHF;
		ar<<m_iAlarmlimitStateDCO2minHF;
		ar<<m_iAlarmlimitMVmaxHF;
		ar<<m_iAlarmlimitStateMVmaxHF;
		ar<<m_iAlarmlimitMVminHF;
		ar<<m_iAlarmlimitStateMVminHF;
		ar<<m_iAlarmlimitPmaxHF;
		ar<<m_iAlarmlimitStatePmaxHF;
		ar<<m_iAlarmlimitPEEPminHF;
		ar<<m_iAlarmlimitStatePEEPminHF;
		ar<<m_iAlarmlimitBPMmaxHF;
		ar<<m_iAlarmlimitStateBPMmaxHF;
		ar<<m_iAlarmlimitLeakmaxHF;
		ar<<m_iAlarmlimitStateLeakmaxHF;
		ar<<m_iAlarmlimitApnoeHF;
		ar<<m_iAlarmlimitStateApnoeHF;
		ar<<m_iAlarmlimitPmaxCpap;
		ar<<m_iAlarmlimitStatePmaxCpap;
		ar<<m_iAlarmlimitPEEPminCpap;
		ar<<m_iAlarmlimitStatePEEPminCpap;
		ar<<(int)m_eCurveForm;
		ar<<m_iPPSVDifference;
		ar<<m_bPPSVasDeltaPEEPValue;
		ar<<m_iAlarmlimitSPO2max;
		ar<<m_iAlarmlimitStateSPO2max;
		ar<<m_iAlarmlimitSPO2min;
		ar<<m_iAlarmlimitStateSPO2min;
		ar<<m_iAlarmlimitPulseRatemax;
		ar<<m_iAlarmlimitStatePulseRatemax;
		ar<<m_iAlarmlimitPulseRatemin;
		ar<<m_iAlarmlimitStatePulseRatemin;
		ar<<m_iAlarmlimitSPO2_PImin;
		ar<<m_iAlarmlimitStateSPO2_PImin;
		ar<<m_iAlarmlimitSPO2maxHF;
		ar<<m_iAlarmlimitStateSPO2maxHF;
		ar<<m_iAlarmlimitSPO2minHF;
		ar<<m_iAlarmlimitStateSPO2minHF;
		ar<<m_iAlarmlimitPulseRatemaxHF;
		ar<<m_iAlarmlimitStatePulseRatemaxHF;
		ar<<m_iAlarmlimitPulseRateminHF;
		ar<<m_iAlarmlimitStatePulseRateminHF;
		ar<<m_iAlarmlimitSPO2_PIminHF;
		ar<<m_iAlarmlimitStateSPO2_PIminHF;
		ar<<m_bFastSATon;
		ar<<(int)m_eSPO2SensitivityMode;
		ar<<(int)m_eSPO2alarmdelay;
		ar<<(int)m_eSPO2averagingTime;

		//##################### m_iConfigVersion 2009
		ar<<m_iCurNumericBlock_IPPV;
		ar<<m_iCurNumericBlock_SIPPV;
		ar<<m_iCurNumericBlock_SIMV;
		ar<<m_iCurNumericBlock_SIMVPSV;
		ar<<m_iCurNumericBlock_PSV;
		ar<<m_iCurNumericBlock_CPAP;
		ar<<m_iCurNumericBlock_HFO;

		//##################### m_iConfigVersion 2010
		ar<<m_iAlarmlimitPIPmaxNCPAP;
		ar<<m_iAlarmlimitStatePmaxNCPAP;
		ar<<m_iAlarmlimitPEEPminNCPAP;
		ar<<m_iAlarmlimitStatePEEPminNCPAP;
		ar<<m_iAlarmlimitPIPmaxDUOPAP;
		ar<<m_iAlarmlimitStatePIPmaxDUOPAP;
		ar<<m_iAlarmlimitPEEPminDUOPAP;
		ar<<m_iAlarmlimitStatePEEPminDUOPAP;


		//##################### m_iConfigVersion 3000
		ar<<m_iParaDataITime_TRIGGER;
		ar<<m_iParaDataETIME_TRIGGER;
		ar<<m_iParaDataBPM_TRIGGER;
		ar<<m_iParaDataRisetime_TRIGGER;
		ar<<m_iParaDataIFlow_TRIGGER;
		ar<<m_iParaDataEFlow_TRIGGER;
		ar<<m_iParaDataPEEP_TRIGGER;
		ar<<m_iParaDataPInsp_TRIGGER;
		ar<<m_iParaDataPmaxVolG_TRIGGER;
		ar<<m_iParaDataVLimit_TRIGGER;
		ar<<m_bParaDataVLimitOn_TRIGGER;
		ar<<m_iParaDataVGarant_TRIGGER;
		ar<<m_bParaDataVGarantOn_TRIGGER;
		ar<<m_bBTPSenable;
		ar<<(int)m_eVControl;
		ar<<m_bFreshgasExt;
		ar<<m_bShowEFlowInSettings;

		//##################### m_iConfigVersion 3001
		ar<<m_iAlarmlimitFICO2max;
		ar<<m_iAlarmlimitStateFICO2max;
		ar<<m_iAlarmlimitFICO2min;
		ar<<m_iAlarmlimitStateFICO2min;
		ar<<m_iAlarmlimitFICO2maxHF;
		ar<<m_iAlarmlimitStateFICO2maxHF;
		ar<<m_iAlarmlimitFICO2minHF;
		ar<<m_iAlarmlimitStateFICO2minHF;
		ar<<m_iParaDataPManual_CPAP;

		//##################### m_iConfigVersion 3003
		ar<<m_iPRICO_SPO2lowRange;
		ar<<m_iPRICO_SPO2highRange;
		ar<<m_iPRICO_FIO2lowRange;
		ar<<m_iPRICO_FIO2highRange;
		ar<<m_iAlarmlimitSPO2_SIQminHF;
		ar<<m_iAlarmlimitStateSPO2_SIQminHF;
		ar<<m_iAlarmlimitSPO2_SIQmin;
		ar<<m_iAlarmlimitStateSPO2_SIQmin;
		
		//##################### m_iConfigVersion 3004
		ar<<m_iFOTconv_AMPLITUDE;
		ar<<m_iFOTconv_FREQ;
		ar<<m_iAlarmlimitStatePIPmin;
		ar<<m_iAlarmlimitPIPmin;

		ar<<m_iFOTconv_PEEPSTART;
		ar<<m_iFOTconv_PEEPEND;
		ar<<m_iFOTconv_STEPS;
		
		ar<<m_iFOThfo_AMPLITUDE;
		ar<<m_iFOThfo_PMEANSTART;
		ar<<m_iFOThfo_PMEANEND;
		ar<<m_iFOThfo_FREQ;
		ar<<m_iFOThfo_STEPS;

		ar<<(BYTE)m_eTriggerType_CONV;
		ar<<(BYTE)m_eTriggerType_NCPAP;
		ar<<m_iParaDataTrigger_NCPAP;

		////##################### m_iConfigVersion 3005
		ar<<(BYTE)m_eLeakCompensation;
		ar<<m_iCurNumericBlock_NCPAP;
		ar<<m_iCurNumericBlock_DUOPAP;
		ar<<m_iCurNumericBlock_THERAPY;
		ar<<m_iCurNumericBlock_FLOWOFFCONV;
		ar<<m_iCurNumericBlock_FLOWOFFHFO;
		ar<<m_bUseNeoPed;
		ar<<m_iCurNumericBlock_FLOWOFFCPAP;
		ar<<m_bEFLOWequalILFOW;
		ar<<(BYTE)m_eAcuLinkVersion;
		ar<<m_iAlarmlimitMAPmaxHF;
		ar<<m_iAlarmlimitStateMAPmaxHF;
		ar<<m_iAlarmlimitMAPminHF;
		ar<<m_iAlarmlimitStateMAPminHF;

		ar<<(BYTE)m_eTriggerType_CPAP;
		ar<<m_iParaDataTrigger_CPAP;

		ar<<(BYTE)m_eTriggerType_DUOPAP;
		ar<<m_iParaDataTrigger_DUOPAP;
		
		////##################### m_iConfigVersion 3006
		ar<<(BYTE)m_ePrevTrigger_CONV;
		ar<<m_iAlarmlimitPIPminDUOPAP;
		ar<<m_iAlarmlimitStatePIPminDUOPAP;
	}
	else
	{
		int iConfigVersion=0;

		ar>>iConfigVersion;

		if(iConfigVersion<=2007)
		{
			//error different FabianHFO-Version
			CString sz=_T("");
			sz.Format(_T("#HFO:0289: %d %d, not possible"), iConfigVersion,m_iConfigVersion);
			theApp.WriteLog(sz);

			getModel()->setLoadHospitalSettingsFailed(true);

			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_LOADHOSPITALSETTINGS_FAIL);
			return;
		}
		else if(iConfigVersion!=m_iConfigVersion)
		{
			CString sz=_T("");
			sz.Format(_T("#HFO:0289: %d %d"), iConfigVersion,m_iConfigVersion);
			theApp.WriteLog(sz);
		}

		ar>>m_iEthernetPort;
		SetEthernetPort(m_iEthernetPort);
		ar>>m_iPDMSProtocol;
		SetPDMSprotocol(m_iPDMSProtocol);
		ar>>m_szLanguageID;
		SetLanguage(m_szLanguageID);
		getModel()->getLANGUAGE()->LoadLang(m_szLanguageID,true);
		ar>>m_iVentRange;
		SetVentRange(m_iVentRange);
		ar>>m_bGraphAutoScale;
		GraphSetAutoScale(m_bGraphAutoScale);
		ar>>m_bTrendAutoScale;
		TrendSetAutoScale(m_bTrendAutoScale);
		ar>>m_bFilledGraph;
		GraphSetFilled(m_bFilledGraph);
		ar>>m_bFilledTrend;
		TrendSetFilled(m_bFilledTrend);
		/*ar>>m_bAskPatDataStartup;
		SetPatDataStartup(m_bAskPatDataStartup);*/
		ar>>m_bUseTveBTB;
		setUseTveBTB(m_bUseTveBTB);

		bool bDummy=false;
		ar>>bDummy;//m_bVolumeTrigger;
		//SetVolumeTrigger(m_bVolumeTrigger);
		// 
		int iIERelationMode=0;
		ar>>iIERelationMode;
		SetIERelationMode((eIERelationMode)iIERelationMode);
		int iRefFlowSensor=0;
		ar>>iRefFlowSensor;
		SetRefFlowSensor((eRefFlowSensor)iRefFlowSensor);
		int iTubeSet=0;
		ar>>iTubeSet;
		SetTubeSet((eTubeSet)iTubeSet);
		int iPressureUnit=0;
		ar>>iPressureUnit;
		SetPressureUnit((ePressureUnit)iPressureUnit);
		ar>>m_bPlayVlimitSound;
		//SetPlayVlimitSound(m_bPlayVlimitSound);
		m_bPlayVlimitSound=false;
		ar>>m_bPlayBackupSound;
		//SetPlayBackupSound(m_bPlayBackupSound);
		m_bPlayBackupSound=false;
		ar>>m_byFlushTime;
		SetFlushTime(m_byFlushTime);
		ar>>m_iManBreathTime;
		SetManBreathTime(m_iManBreathTime);
		ar>>m_iNebulizerTime;
		SetNebulizerTime(m_iNebulizerTime);
		ar>>m_iPminAlarmDelay;
		SetCurPminAlarmDelay(m_iPminAlarmDelay);
		ar>>m_iPatientAlarmDelay;
		setCurPatientAlarmDelay(m_iPatientAlarmDelay);
		ar>>m_bHFOManBreathEnabled;
		SetHFOManBreathEnabled(m_bHFOManBreathEnabled);
		ar>>m_iPercentAbortCriterionPSV;
		SetPercentAbortCriterionPSV(m_iPercentAbortCriterionPSV);
		int iAutoOxyCal=0;
		ar>>iAutoOxyCal;
		SetAutoOxyCal((eAutoOxyCal)iAutoOxyCal);
		int iAutoScreenlocktime=0;
		ar>>iAutoScreenlocktime;
		SetAutoScreenlockTime((eAutoScreenlock)iAutoScreenlocktime);
		ar>>m_bAutoScreenlockActive;
		SetAutoScreenlockActive(m_bAutoScreenlockActive);
		eCO2unit unitOld=m_eCO2unit;
		int iCO2unit=0;
		ar>>iCO2unit;
		m_eCO2unit=(eCO2unit)iCO2unit;
		if(unitOld!=m_eCO2unit)
		{
			if(getCO2module()!=CO2MODULE_NONE && getModel()->getETCO2()!=NULL)
			{
				getModel()->getETCO2()->changeCO2Unit(m_eCO2unit,unitOld);
			}
			else
				getModel()->getI2C()->WriteConfigByte(CO2UNIT_8, (int)m_eCO2unit);
		}

		ar>>m_iCO2BaroPressure;
		getModel()->getI2C()->WriteConfigWord(CO2BAROPRESSURE_16, m_iCO2BaroPressure);
		if(getCO2module()!=CO2MODULE_NONE && getModel()->getETCO2()!=NULL)
		{
			getModel()->getETCO2()->set_BaroPressure(m_iCO2BaroPressure);
		}
		ar>>m_iCO2TimePumpAutoOn;
		SetTimePumpAutoOn(m_iCO2TimePumpAutoOn);
		
		ar>>m_bServicePasswordDisabled;
		SetServicePasswordDisabled(m_bServicePasswordDisabled);
		ar>>m_bNursecallActive;
		SetNursecallActive(m_bNursecallActive);
		ar>>m_bEnableAutolimit;
		SetEnableAutolimit(m_bEnableAutolimit);
		ar>>m_bEnableFactory;
		SetEnableFactory(m_bEnableFactory);
		int iAlarmVolume=0;
		ar>>iAlarmVolume;
		SetAlarmVolume((eAlarmLoudness)iAlarmVolume);
		ar>>m_iTypeTrend1;
		SetTrendType1(m_iTypeTrend1);
		ar>>m_iTypeTrend2;
		SetTrendType2(m_iTypeTrend2);
		ar>>m_iTypeTrend3;
		SetTrendType3(m_iTypeTrend3);

		ar>>m_iParaDataITime_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_ITIME_IPPV_16, m_iParaDataITime_IPPV);
		ar>>m_iParaDataITime_NMODE;
		getModel()->getI2C()->WriteConfigWord(PARA_ITIME_NMODE_16, m_iParaDataITime_NMODE);
		ar>>m_iParaDataETIME_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_ETIME_IPPV_16, m_iParaDataETIME_IPPV);
		ar>>m_iParaDataETIME_NMODE;
		getModel()->getI2C()->WriteConfigWord(PARA_ETIME_NMODE_16, m_iParaDataETIME_NMODE);
		ar>>m_iParaDataBPM_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_BPM_IPPV_16, m_iParaDataBPM_IPPV);
		ar>>m_iParaDataBPM_NMODE;
		getModel()->getI2C()->WriteConfigWord(PARA_BPM_NMODE_16, m_iParaDataBPM_NMODE);
		ar>>m_iParaDataRisetime_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_RISETIME_IPPV_16, m_iParaDataRisetime_IPPV);
		ar>>m_iParaDataIFlow_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_IPPV_16, m_iParaDataIFlow_IPPV);
		ar>>m_iParaDataHFFlow;
		getModel()->getI2C()->WriteConfigWord(PARA_HFFLOW_16, m_iParaDataHFFlow);
		ar>>m_iParaDataEFlow_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_IPPV_16, m_iParaDataEFlow_IPPV);
		ar>>m_iParaDataTherapieFlow;
		getModel()->getI2C()->WriteConfigWord(PARA_THERAPYFLOW_16, m_iParaDataTherapieFlow);
		ar>>m_iParaDataFlowMin;
		getModel()->getI2C()->WriteConfigWord(PARA_FLOWMIN_16, m_iParaDataFlowMin);
		ar>>m_iParaDataPEEP_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_PEEP_IPPV_16, m_iParaDataPEEP_IPPV);
		ar>>m_iParaDataCpap;
		getModel()->getI2C()->WriteConfigWord(PARA_CPAP_16, m_iParaDataCpap);
		ar>>m_iParaDataNmode;
		getModel()->getI2C()->WriteConfigWord(PARA_CPAP_NMODE_16, m_iParaDataNmode);
		ar>>m_iParaDataPInsp_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_PINSP_IPPV_16, m_iParaDataPInsp_IPPV);
		ar>>m_iParaDataPmaxVolG_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_IPPV_16, m_iParaDataPmaxVolG_IPPV);
		ar>>m_iParaDataPPSV;
		getModel()->getI2C()->WriteConfigWord(PARA_PPSV_16, m_iParaDataPPSV);
		ar>>m_iParaDataPManual_HFO;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAN_HFO_16, m_iParaDataPManual_HFO);
		ar>>m_iParaDataPManual_NMODE;
		getModel()->getI2C()->WriteConfigWord(PARA_PMAN_NMODE_16, m_iParaDataPManual_NMODE);
		ar>>m_iParaDataO2;
		getModel()->getI2C()->WriteConfigWord(PARA_O2_16, m_iParaDataO2);
		ar>>m_iParaDataO2Flush;
		getModel()->getI2C()->WriteConfigWord(PARA_O2FLUSH_16, m_iParaDataO2Flush);
		ar>>m_iO2Difference;
		getModel()->getI2C()->WriteConfigWord(PARA_O2DIFF_16, m_iO2Difference);
		ar>>m_iPmeanDifference;
		getModel()->getI2C()->WriteConfigWord(PARA_PMEANDIFF_16, m_iPmeanDifference);
		ar>>m_iParaDataVLimit_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_IPPV_16, m_iParaDataVLimit_IPPV);
		ar>>m_bParaDataVLimitOn_IPPV;
		if(m_bParaDataVLimitOn_IPPV)
		{
			getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_IPPV_8, 1);
		}
		else
		{
			getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_IPPV_8, 0);
		}
		ar>>m_iParaDataVGarant_IPPV;
		getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_IPPV_16, m_iParaDataVGarant_IPPV);
		ar>>m_bParaDataVGarantOn_IPPV;
		if(m_bParaDataVGarantOn_IPPV)
		{
			getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_IPPV_8, 1);
		}
		else
		{
			getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_IPPV_8, 0);
		}
		ar>>m_iParaDataTrigger_CONV;
		getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CONVENT_16, m_iParaDataTrigger_CONV);
		ar>>m_iParaDataBackup;
		getModel()->getI2C()->WriteConfigWord(PARA_BACKUP_16, m_iParaDataBackup);
		ar>>m_iParaDataHFAmpl;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFAMPL_16, m_iParaDataHFAmpl);
		ar>>m_iParaDataHFAmplmax;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFAMPL_VOLG_16, m_iParaDataHFAmplmax);
		ar>>m_iParaDataHFFreq;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_HFFREQ_16, m_iParaDataHFFreq);
		ar>>m_iParaDataHFPmean;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_PMEAN_16, m_iParaDataHFPmean);
		ar>>m_iParaDataHFVGarant;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_VGARANT_16, m_iParaDataHFVGarant);
		ar>>m_bParaDataHFVGarantState;
		if(m_bParaDataHFVGarantState)
		{
			getModel()->getI2C()->WriteConfigByte(PARA_HFO_VGARANTSTATE_8, 1);
		}
		else
		{
			getModel()->getI2C()->WriteConfigByte(PARA_HFO_VGARANTSTATE_8, 0);
		}
		int iTempRatio=0;
		ar>>iTempRatio;
		m_iParaDataHFIERatio=(eRatioIE)iTempRatio;
		getModel()->getI2C()->WriteConfigByte(PARA_HFO_IERATIO_8, m_iParaDataHFIERatio);

		ar>>m_iParaDataHFITimeRec;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_ITIMEREC16, m_iParaDataHFITimeRec);
		ar>>m_iParaDataHFFREQ_REC;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_FREQREC16, m_iParaDataHFFREQ_REC);
		ar>>m_iParaDataHFPMeanRec;
		getModel()->getI2C()->WriteConfigWord(PARA_HFO_PMEANREC16, m_iParaDataHFPMeanRec);
		

		ar>>m_iAlarmlimitETCO2max;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_16, m_iAlarmlimitETCO2max);
		ar>>m_iAlarmlimitStateETCO2max;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MAX_8, m_iAlarmlimitStateETCO2max);
		ar>>m_iAlarmlimitETCO2min;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_16, m_iAlarmlimitETCO2min);
		ar>>m_iAlarmlimitStateETCO2min;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MIN_8, m_iAlarmlimitStateETCO2min);
		ar>>m_iAlarmlimitMVmax;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMAX_16, m_iAlarmlimitMVmax);
		ar>>m_iAlarmlimitStateMVmax;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMAX_8, m_iAlarmlimitStateMVmax);
		ar>>m_iAlarmlimitMVmin;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMIN_16, m_iAlarmlimitMVmin);
		ar>>m_iAlarmlimitStateMVmin;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMIN_8, m_iAlarmlimitStateMVmin);
		ar>>m_iAlarmlimitPIPmax;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PIPMAX_16, m_iAlarmlimitPIPmax);
		ar>>m_iAlarmlimitStatePIPmax;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PIPMAX_8, m_iAlarmlimitStatePIPmax);
		ar>>m_iAlarmlimitPEEPmin;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_16, m_iAlarmlimitPEEPmin);
		ar>>m_iAlarmlimitStatePEEPmin;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_8, m_iAlarmlimitStatePEEPmin);
		ar>>m_iAlarmlimitBPMmax;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_BPMMAX_16, m_iAlarmlimitBPMmax);
		ar>>m_iAlarmlimitStateBPMmax;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_BPMMAX_8, m_iAlarmlimitStateBPMmax);
		ar>>m_iAlimitLeakmax;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_LEAKMAX_16, m_iAlimitLeakmax);
		ar>>m_iAlarmlimitStateLeakmax;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_LEAKMAX_8, m_iAlarmlimitStateLeakmax);
		ar>>m_iAlarmlimitApnoe;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_APNOE_16, m_iAlarmlimitApnoe);
		ar>>m_iAlarmlimitStateApnoe;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_APNOE_8, m_iAlarmlimitStateApnoe);
		ar>>m_iAlarmlimitETCO2maxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MAX_HF_16, m_iAlarmlimitETCO2maxHF);
		ar>>m_iAlarmlimitStateETCO2maxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MAX_HF_8, m_iAlarmlimitStateETCO2maxHF);
		ar>>m_iAlarmlimitETCO2minHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_ETCO2MIN_HF_16, m_iAlarmlimitETCO2minHF);
		ar>>m_iAlarmlimitStateETCO2minHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_ETCO2MIN_HF_8, m_iAlarmlimitStateETCO2minHF);
		ar>>m_iAlarmlimitDCO2maxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_DCO2MAX_HF_16, m_iAlarmlimitDCO2maxHF);
		ar>>m_iAlarmlimitStateDCO2maxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_DCO2MAX_HF_8, m_iAlarmlimitStateDCO2maxHF);
		ar>>m_iAlarmlimitDCO2minHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_DCO2MIN_HF_16, m_iAlarmlimitDCO2minHF);
		ar>>m_iAlarmlimitStateDCO2minHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_DCO2MIN_HF_8, m_iAlarmlimitStateDCO2minHF);
		ar>>m_iAlarmlimitMVmaxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMAX_HF_16, m_iAlarmlimitMVmaxHF);
		ar>>m_iAlarmlimitStateMVmaxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMAX_HF_8, m_iAlarmlimitStateMVmaxHF);
		ar>>m_iAlarmlimitMVminHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MVMIN_HF_16, m_iAlarmlimitMVminHF);
		ar>>m_iAlarmlimitStateMVminHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MVMIN_HF_8, m_iAlarmlimitStateMVminHF);
		ar>>m_iAlarmlimitPmaxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_HF_16, m_iAlarmlimitPmaxHF);
		ar>>m_iAlarmlimitStatePmaxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_HF_8, m_iAlarmlimitStatePmaxHF);
		ar>>m_iAlarmlimitPEEPminHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_HF_16, m_iAlarmlimitPEEPminHF);
		ar>>m_iAlarmlimitStatePEEPminHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_HF_8, m_iAlarmlimitStatePEEPminHF);
		ar>>m_iAlarmlimitBPMmaxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_BPMMAX_HF_16, m_iAlarmlimitBPMmaxHF);
		ar>>m_iAlarmlimitStateBPMmaxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_BPMMAX_HF_8, m_iAlarmlimitStateBPMmaxHF);
		ar>>m_iAlarmlimitLeakmaxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_LEAKMAX_HF_16, m_iAlarmlimitLeakmaxHF);
		ar>>m_iAlarmlimitStateLeakmaxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_LEAKMAX_HF_8, m_iAlarmlimitStateLeakmaxHF);
		ar>>m_iAlarmlimitApnoeHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_APNOE_HF_16, m_iAlarmlimitApnoeHF);
		ar>>m_iAlarmlimitStateApnoeHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_APNOE_HF_8, m_iAlarmlimitStateApnoeHF);
		ar>>m_iAlarmlimitPmaxCpap;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_CPAP_16, m_iAlarmlimitPmaxCpap);
		ar>>m_iAlarmlimitStatePmaxCpap;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_CPAP_8, m_iAlarmlimitStatePmaxCpap);
		ar>>m_iAlarmlimitPEEPminCpap;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_CPAP_16, m_iAlarmlimitPEEPminCpap);
		ar>>m_iAlarmlimitStatePEEPminCpap;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_CPAP_8, m_iAlarmlimitStatePEEPminCpap);
		int form=0;
		ar>>form;
		m_eCurveForm=(eCurveForm)form;
		//if(NEONATAL==m_iVentRange && CURVE_LINEAR==m_eCurveForm)//RISE_NEO
		//{
		//	m_eCurveForm=CURVE_IFLOW;
		//}
		getModel()->getI2C()->WriteConfigByte(PRESSURERISECTRL_8, (int)m_eCurveForm);
		ar>>m_iPPSVDifference;
		getModel()->getI2C()->WriteConfigWord(PARA_PPSVDIFF_16, m_iPPSVDifference);
		ar>>m_bPPSVasDeltaPEEPValue;
		getModel()->getI2C()->WriteConfigByte(PPSV_AS_PEEPDELTA_8, m_bPPSVasDeltaPEEPValue);
		ar>>m_iAlarmlimitSPO2max;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_16, m_iAlarmlimitSPO2max);
		ar>>m_iAlarmlimitStateSPO2max;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MAX_8,m_iAlarmlimitStateSPO2max);
		ar>>m_iAlarmlimitSPO2min;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_16, m_iAlarmlimitSPO2min);
		ar>>m_iAlarmlimitStateSPO2min;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MIN_8,m_iAlarmlimitStateSPO2min);
		ar>>m_iAlarmlimitPulseRatemax;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_16, m_iAlarmlimitPulseRatemax);
		ar>>m_iAlarmlimitStatePulseRatemax;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMAX_8,m_iAlarmlimitStatePulseRatemax);
		ar>>m_iAlarmlimitPulseRatemin;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_16, m_iAlarmlimitPulseRatemin);
		ar>>m_iAlarmlimitStatePulseRatemin;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMIN_8,m_iAlarmlimitStatePulseRatemin);
		ar>>m_iAlarmlimitSPO2_PImin;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_16, m_iAlarmlimitSPO2_PImin);
		ar>>m_iAlarmlimitStateSPO2_PImin;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_PIMIN_8,m_iAlarmlimitStateSPO2_PImin);
		ar>>m_iAlarmlimitSPO2maxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MAX_HF_16,m_iAlarmlimitSPO2maxHF);
		ar>>m_iAlarmlimitStateSPO2maxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MAX_HF_8,m_iAlarmlimitStateSPO2maxHF);
		ar>>m_iAlarmlimitSPO2minHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2MIN_HF_16,m_iAlarmlimitSPO2minHF);
		ar>>m_iAlarmlimitStateSPO2minHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2MIN_HF_8,m_iAlarmlimitStateSPO2minHF);
		ar>>m_iAlarmlimitPulseRatemaxHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMAX_HF_16,m_iAlarmlimitPulseRatemaxHF);
		ar>>m_iAlarmlimitStatePulseRatemaxHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMAX_HF_8,m_iAlarmlimitStatePulseRatemaxHF);
		ar>>m_iAlarmlimitPulseRateminHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PULSERATEMIN_HF_16,m_iAlarmlimitPulseRateminHF);
		ar>>m_iAlarmlimitStatePulseRateminHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PULSERATEMIN_HF_8,m_iAlarmlimitStatePulseRateminHF);
		ar>>m_iAlarmlimitSPO2_PIminHF;
		getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_PIMIN_HF_16,m_iAlarmlimitSPO2_PIminHF);
		ar>>m_iAlarmlimitStateSPO2_PIminHF;
		getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_PIMIN_HF_8,m_iAlarmlimitStateSPO2_PIminHF);

		ar>>m_bFastSATon;
		if(m_bFastSATon)
			getModel()->getI2C()->WriteConfigByte(SPO2FASTSAT_8, 1);
		else
			getModel()->getI2C()->WriteConfigByte(SPO2FASTSAT_8, 0);
		
		int iSPO2SensitivityMode=0;
		ar>>iSPO2SensitivityMode;
		m_eSPO2SensitivityMode=(eSPO2sensitivitymode)iSPO2SensitivityMode;
		getModel()->getI2C()->WriteConfigByte(SPO2SENSITIVITY_8, (BYTE)m_eSPO2SensitivityMode);

		int iSPO2delay=0;
		ar>>iSPO2delay;
		m_eSPO2alarmdelay=(eSPO2alarmdelay)iSPO2delay;
		getModel()->getI2C()->WriteConfigByte(SPO2ALARMDELAY_8,(BYTE)m_eSPO2alarmdelay);

		int iSPO2averagingTime=0;
		ar>>iSPO2averagingTime;
		m_eSPO2averagingTime=(eSPO2averagingtime)iSPO2averagingTime;
		getModel()->getI2C()->WriteConfigByte(SPO2AVERAGINGTIME_8, (BYTE)m_eSPO2averagingTime);

		//##################### m_iConfigVersion 2009
		if(iConfigVersion>=2009)
		{
			ar>>m_iCurNumericBlock_IPPV;
			setLastNumericIPPV(m_iCurNumericBlock_IPPV);

			ar>>m_iCurNumericBlock_SIPPV;
			setLastNumericSIPPV(m_iCurNumericBlock_SIPPV);

			ar>>m_iCurNumericBlock_SIMV;
			setLastNumericSIMV(m_iCurNumericBlock_SIMV);

			ar>>m_iCurNumericBlock_SIMVPSV;
			setLastNumericSIMVPSV(m_iCurNumericBlock_SIMVPSV);

			ar>>m_iCurNumericBlock_PSV;
			setLastNumericPSV(m_iCurNumericBlock_PSV);

			ar>>m_iCurNumericBlock_CPAP;
			setLastNumericCPAP(m_iCurNumericBlock_CPAP);

			ar>>m_iCurNumericBlock_HFO;
			setLastNumericHFO(m_iCurNumericBlock_HFO);
		}
		//##################### m_iConfigVersion 2010
		if(iConfigVersion>=2010)
		{	
			ar>>m_iAlarmlimitPIPmaxNCPAP;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_NCPAP_16, m_iAlarmlimitPIPmaxNCPAP);
			ar>>m_iAlarmlimitStatePmaxNCPAP;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_NCPAP_8, m_iAlarmlimitStatePmaxNCPAP);
			ar>>m_iAlarmlimitPEEPminNCPAP;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_NCPAP_16, m_iAlarmlimitPEEPminNCPAP);
			ar>>m_iAlarmlimitStatePEEPminNCPAP;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_NCPAP_8, m_iAlarmlimitStatePEEPminNCPAP);


			ar>>m_iAlarmlimitPIPmaxDUOPAP;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMAX_DUOPAP_16, m_iAlarmlimitPIPmaxDUOPAP);
			ar>>m_iAlarmlimitStatePIPmaxDUOPAP;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMAX_DUOPAP_8, m_iAlarmlimitStatePIPmaxDUOPAP);
			ar>>m_iAlarmlimitPEEPminDUOPAP;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PEEP_DUOPAP_16, m_iAlarmlimitPEEPminDUOPAP);
			ar>>m_iAlarmlimitStatePEEPminDUOPAP;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PEEP_DUOPAP_8, m_iAlarmlimitStatePEEPminDUOPAP);
		}



		//##################### m_iConfigVersion 3000
		if(iConfigVersion>=3000)
		{
			ar>>m_iParaDataITime_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_ITIME_TRIGGER_16, m_iParaDataITime_TRIGGER);
			ar>>m_iParaDataETIME_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_ETIME_TRIGGER_16, m_iParaDataETIME_TRIGGER);
			ar>>m_iParaDataBPM_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_BPM_TRIGGER_16, m_iParaDataBPM_TRIGGER);
			ar>>m_iParaDataRisetime_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_RISETIME_TRIGGER_16, m_iParaDataRisetime_TRIGGER);
			ar>>m_iParaDataIFlow_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_IFLOW_TRIGGER_16, m_iParaDataIFlow_TRIGGER);
			ar>>m_iParaDataEFlow_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_EFLOW_TRIGGER_16, m_iParaDataEFlow_TRIGGER);
			ar>>m_iParaDataPEEP_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_PEEP_TRIGGER_16, m_iParaDataPEEP_TRIGGER);
			ar>>m_iParaDataPInsp_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_PINSP_TRIGGER_16, m_iParaDataPInsp_TRIGGER);
			ar>>m_iParaDataPmaxVolG_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_PMAXVOLG_TRIGGER_16, m_iParaDataPmaxVolG_TRIGGER);

			ar>>m_iParaDataVLimit_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_VLIMIT_TRIGGER_16, m_iParaDataVLimit_TRIGGER);
			ar>>m_bParaDataVLimitOn_TRIGGER;
			if(m_bParaDataVLimitOn_TRIGGER)
			{
				getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 1);
			}
			else
			{
				getModel()->getI2C()->WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8, 0);
			}
			ar>>m_iParaDataVGarant_TRIGGER;
			getModel()->getI2C()->WriteConfigWord(PARA_VGARANT_TRIGGER_16, m_iParaDataVGarant_TRIGGER);
			ar>>m_bParaDataVGarantOn_TRIGGER;
			if(m_bParaDataVGarantOn_TRIGGER)
			{
				getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 1);
			}
			else
			{
				getModel()->getI2C()->WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8, 0);
			}
			ar>>m_bBTPSenable;
			if(m_bBTPSenable)
			{
				getModel()->getI2C()->WriteConfigByte(BTPS_8, 1);
			}
			else
			{
				getModel()->getI2C()->WriteConfigByte(BTPS_8, 0);
			}
			int iVControl=0;
			ar>>iVControl;
			SetVolumeControl((eVolumeControl)iVControl);


			ar>>m_bFreshgasExt;
			if(m_bFreshgasExt)
			{
				getModel()->getI2C()->WriteConfigByte(FRESHGASEXT_8, 1);
			}
			else
			{
				getModel()->getI2C()->WriteConfigByte(FRESHGASEXT_8, 0);
			}

			ar>>m_bShowEFlowInSettings;
			if(m_bShowEFlowInSettings)
			{
				getModel()->getI2C()->WriteConfigByte(EFLOWSETTINGS_8, 1);
			}
			else
			{
				getModel()->getI2C()->WriteConfigByte(EFLOWSETTINGS_8, 0);
			}
		}

		//##################### m_iConfigVersion 3001
		if(iConfigVersion>=3001)
		{
			ar>>m_iAlarmlimitFICO2max;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_16, m_iAlarmlimitFICO2max);
			ar>>m_iAlarmlimitStateFICO2max;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MAX_8, m_iAlarmlimitStateFICO2max);
			ar>>m_iAlarmlimitFICO2min;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_16, m_iAlarmlimitFICO2min);
			ar>>m_iAlarmlimitStateFICO2min;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MIN_8, m_iAlarmlimitStateFICO2min);
			ar>>m_iAlarmlimitFICO2maxHF;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MAX_HF_16, m_iAlarmlimitFICO2maxHF);
			ar>>m_iAlarmlimitStateFICO2maxHF;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MAX_HF_8, m_iAlarmlimitStateFICO2maxHF);
			ar>>m_iAlarmlimitFICO2minHF;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_FICO2MIN_HF_16, m_iAlarmlimitFICO2minHF);
			ar>>m_iAlarmlimitStateFICO2minHF;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_FICO2MIN_HF_8, m_iAlarmlimitStateFICO2minHF);
			ar>>m_iParaDataPManual_CPAP;
			getModel()->getI2C()->WriteConfigWord(PARA_PMAN_CPAP_16, m_iParaDataPManual_CPAP);
		}

		//##################### m_iConfigVersion 3003
		if(iConfigVersion>=3003)
		{
			ar>>m_iPRICO_SPO2lowRange;
			getModel()->getI2C()->WriteConfigByte(PRICOSPO2LOW_8,m_iPRICO_SPO2lowRange);
			ar>>m_iPRICO_SPO2highRange;
			getModel()->getI2C()->WriteConfigByte(PRICOSPO2HIGH_8,m_iPRICO_SPO2highRange);
			
			//PRICO02
			ar>>m_iPRICO_FIO2lowRange;
			/*if(m_iPRICO_FIO2lowRange>m_iParaDataO2)
			{
				m_iPRICO_FIO2lowRange=m_iParaDataO2;
			}*/
			getModel()->getI2C()->WriteConfigByte(PRICOFIO2LOW_8,m_iPRICO_FIO2lowRange);
			ar>>m_iPRICO_FIO2highRange;
			/*if(m_iPRICO_FIO2highRange<m_iParaDataO2)
			{
				m_iPRICO_FIO2highRange=m_iParaDataO2;
			}*/
			getModel()->getI2C()->WriteConfigByte(PRICOFIO2HIGH_8,m_iPRICO_FIO2highRange);

			ar>>m_iAlarmlimitSPO2_SIQminHF;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_SIQMIN_HF_16,m_iAlarmlimitSPO2_SIQminHF);
			ar>>m_iAlarmlimitStateSPO2_SIQminHF;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_SIQMIN_HF_8,m_iAlarmlimitStateSPO2_SIQminHF);
			ar>>m_iAlarmlimitSPO2_SIQmin;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_SPO2_SIQMIN_16, m_iAlarmlimitSPO2_SIQmin);
			ar>>m_iAlarmlimitStateSPO2_SIQmin;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_SPO2_SIQMIN_8,m_iAlarmlimitStateSPO2_SIQmin);
		}
		//##################### m_iConfigVersion 3004
		if(iConfigVersion>=3004)
		{
			ar>>m_iFOTconv_AMPLITUDE;
			getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_AMPLITUDE_16, m_iFOTconv_AMPLITUDE);
			ar>>m_iFOTconv_FREQ;
			getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_FREQ_8, m_iFOTconv_FREQ);
			ar>>m_iAlarmlimitStatePIPmin;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PIPMIN_8, m_iAlarmlimitStatePIPmin);
			ar>>m_iAlarmlimitPIPmin;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PIPMIN_16, m_iAlarmlimitPIPmin);

			ar>>m_iFOTconv_PEEPSTART;
			getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPSTART_16,m_iFOTconv_PEEPSTART);
			ar>>m_iFOTconv_PEEPEND;
			getModel()->getI2C()->WriteConfigWord(PARA_FOT_CONV_PEEPEND_16,m_iFOTconv_PEEPEND);
			ar>>m_iFOTconv_STEPS;
			getModel()->getI2C()->WriteConfigByte(PARA_FOT_CONV_STEPS_8,m_iFOTconv_STEPS);
			
			ar>>m_iFOThfo_AMPLITUDE;
			getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_AMPLITUDE_16, m_iFOThfo_AMPLITUDE);
			ar>>m_iFOThfo_PMEANSTART;
			getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANSTART_16,m_iFOThfo_PMEANSTART);
			ar>>m_iFOThfo_PMEANEND;
			getModel()->getI2C()->WriteConfigWord(PARA_FOT_HFO_PMEANEND_16,m_iFOThfo_PMEANEND);
			ar>>m_iFOThfo_FREQ;
			getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_FREQ_8, m_iFOThfo_FREQ);
			ar>>m_iFOThfo_STEPS;
			getModel()->getI2C()->WriteConfigByte(PARA_FOT_HFO_STEPS_8,m_iFOThfo_STEPS);
			
			BYTE eTriggerTypeCONV=0;
			ar>>eTriggerTypeCONV;
			m_eTriggerType_CONV=(eTriggereType)eTriggerTypeCONV;
			setTriggerOption_CONV(m_eTriggerType_CONV);
			
			BYTE eTriggerTypeNCPAP=0;
			ar>>eTriggerTypeNCPAP;
			m_eTriggerType_NCPAP=(eTriggereType)eTriggerTypeNCPAP;
			setTriggerOption_NCPAP(m_eTriggerType_NCPAP);

			ar>>m_iParaDataTrigger_NCPAP;
			getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_NCPAP_16, m_iParaDataTrigger_NCPAP);

			if(m_iParaDataTrigger_NCPAP!=MAXRANGE_TRIGGER_OFF && getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable()==false)
				disableNIVTRIGGER();
		}

		//##################### m_iConfigVersion 3005
		if(iConfigVersion>=3005)
		{
			BYTE eLeakCompOff=0;
			ar>>eLeakCompOff;
			m_eLeakCompensation=(eLeakCompensation)eLeakCompOff;
			getModel()->getI2C()->WriteConfigByte(LEAKCOMPENSATIONOFF_8, (int)m_eLeakCompensation);

			ar>>m_iCurNumericBlock_NCPAP;
			setLastNumericNCPAP(m_iCurNumericBlock_NCPAP);

			ar>>m_iCurNumericBlock_DUOPAP;
			setLastNumericDUOPAP(m_iCurNumericBlock_DUOPAP);

			ar>>m_iCurNumericBlock_THERAPY;
			setLastNumericTHERAPY(m_iCurNumericBlock_THERAPY);

			ar>>m_iCurNumericBlock_FLOWOFFCONV;
			setLastNumericFLOWOFFCONV(m_iCurNumericBlock_FLOWOFFCONV);

			ar>>m_iCurNumericBlock_FLOWOFFHFO;
			setLastNumericFLOWOFFHFO(m_iCurNumericBlock_FLOWOFFHFO);

			ar>>m_bUseNeoPed;
			setUseNeoPed(m_bUseNeoPed);

			ar>>m_iCurNumericBlock_FLOWOFFCPAP;
			setLastNumericFLOWOFFCPAP(m_iCurNumericBlock_FLOWOFFCPAP);

			ar>>m_bEFLOWequalILFOW;
			SetEFLOWequalILFOW(m_bEFLOWequalILFOW);

			BYTE byVersion=0;
			ar>>byVersion;
			m_eAcuLinkVersion=(eAcuLinkVersion)byVersion;
			SetAcuLinkVersion(m_eAcuLinkVersion);

			ar>>m_iAlarmlimitMAPmaxHF;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MAPMAX_HF_16, m_iAlarmlimitMAPmaxHF);
			ar>>m_iAlarmlimitStateMAPmaxHF;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MAPMAX_HF_8, m_iAlarmlimitStateMAPmaxHF);
			ar>>m_iAlarmlimitMAPminHF;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_MAPMIN_HF_16, m_iAlarmlimitMAPminHF);
			ar>>m_iAlarmlimitStateMAPminHF;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_MAPMIN_HF_8, m_iAlarmlimitStateMAPminHF);

			//###########################

			BYTE eTriggerTypeCPAP=0;
			ar>>eTriggerTypeCPAP;
			m_eTriggerType_CPAP=(eTriggereType)eTriggerTypeCPAP;
			setTriggerOption_CPAP(m_eTriggerType_CPAP);

			ar>>m_iParaDataTrigger_CPAP;
			getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_CPAP_16, m_iParaDataTrigger_CPAP);

			//#############################
			BYTE eTriggerTypeDUOPAP=0;
			ar>>eTriggerTypeDUOPAP;
			m_eTriggerType_DUOPAP=(eTriggereType)eTriggerTypeDUOPAP;

			if(m_eTubeSet==TUBE_INFANTFLOW)
				m_eTriggerType_DUOPAP=TRIGGER_FLOW;
			else if(m_eTubeSet==TUBE_INFANTFLOW_LP)
				m_eTriggerType_DUOPAP=TRIGGER_FLOW;
			else if(m_eTubeSet==TUBE_MEDIJET)
				m_eTriggerType_DUOPAP=TRIGGER_PRESSURE;

			setTriggerOption_DUOPAP(m_eTriggerType_DUOPAP);

			ar>>m_iParaDataTrigger_DUOPAP;
			getModel()->getI2C()->WriteConfigWord(PARA_TRIGGER_DUOPAP_16, m_iParaDataTrigger_DUOPAP);

			if(m_iParaDataTrigger_DUOPAP!=MAXRANGE_TRIGGER_OFF && getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable()==false)
				disableNIVTRIGGER();
		}

		//##################### m_iConfigVersion 3006
		if(iConfigVersion>=3006)
		{
			BYTE ePrevTriggerTypeCONV=0;
			ar>>ePrevTriggerTypeCONV;
			m_ePrevTrigger_CONV=(eTriggereType)ePrevTriggerTypeCONV;
			setTriggerOption_CONV(m_ePrevTrigger_CONV);

			ar>>m_iAlarmlimitPIPminDUOPAP;
			getModel()->getI2C()->WriteConfigWord(ALIMIT_VAL_PMIN_DUOPAP_16, m_iAlarmlimitPIPminDUOPAP);
			ar>>m_iAlarmlimitStatePIPminDUOPAP;
			getModel()->getI2C()->WriteConfigByte(ALIMIT_STATE_PMIN_DUOPAP_8, m_iAlarmlimitStatePIPminDUOPAP);
		}
	}
}



/**********************************************************************************************//**
 * Gets configuration version
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The configuration version.
 **************************************************************************************************/

int CConfiguration::getConfigVersion()
{
	return m_iConfigVersion;
}

/**********************************************************************************************//**
 * Gets the last numeric ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric ippv.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericIPPV()
{
	return m_iCurNumericBlock_IPPV;
}

/**********************************************************************************************//**
 * Sets last numeric ippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericIPPV(BYTE num)
{
	m_iCurNumericBlock_IPPV=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_IPPV_8, num);
}

/**********************************************************************************************//**
 * Gets the last numeric sippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric sippv.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericSIPPV()
{
	return m_iCurNumericBlock_SIPPV;
}

/**********************************************************************************************//**
 * Sets last numeric sippv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericSIPPV(BYTE num)
{
	m_iCurNumericBlock_SIPPV=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_SIPPV_8, num);
}

/**********************************************************************************************//**
 * Gets the last numeric simv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric simv.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericSIMV()
{
	return m_iCurNumericBlock_SIMV;
}

/**********************************************************************************************//**
 * Sets last numeric simv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericSIMV(BYTE num)
{
	m_iCurNumericBlock_SIMV=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_SIMV_8, num);
}

/**********************************************************************************************//**
 * Gets the last numeric simvpsv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric simvpsv.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericSIMVPSV()
{
	return m_iCurNumericBlock_SIMVPSV;
}

/**********************************************************************************************//**
 * Sets last numeric simvpsv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericSIMVPSV(BYTE num)
{
	m_iCurNumericBlock_SIMVPSV=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_SIMVPSV_8, num);
}

/**********************************************************************************************//**
 * Gets the last numeric psv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric psv.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericPSV()
{
	return m_iCurNumericBlock_PSV;
}

/**********************************************************************************************//**
 * Sets last numeric psv
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericPSV(BYTE num)
{
	m_iCurNumericBlock_PSV=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_PSV_8, num);
}

/**********************************************************************************************//**
 * Gets the last numeric cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric cpap.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericCPAP()
{
	return m_iCurNumericBlock_CPAP;
}

/**********************************************************************************************//**
 * Sets last numeric cpap
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericCPAP(BYTE num)
{
	m_iCurNumericBlock_CPAP=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_CPAP_8, num);
}

/**********************************************************************************************//**
 * Gets the last numeric hfo
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric hfo.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericHFO()
{
	return m_iCurNumericBlock_HFO;
}

/**********************************************************************************************//**
 * Sets last numeric hfo
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericHFO(BYTE num)
{
	m_iCurNumericBlock_HFO=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_HFO_8, num);
}

/**********************************************************************************************//**
 * @fn	BYTE CConfiguration::getLastNumericNCPAP()
 * 
 * @brief	Gets the last numeric ncpap.
 * 
 * @author	Rainer Kuehner
 * @date	08.02.2017
 * 
 * @return	The last numeric ncpap.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric ncpap.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericNCPAP()
{
	return m_iCurNumericBlock_NCPAP;
}

/**********************************************************************************************//**
 * @fn	void CConfiguration::setLastNumericNCPAP(BYTE num)
 * 
 * @brief	Sets last numeric ncpap.
 * 
 * @author	Rainer Kuehner
 * @date	08.02.2017
 * 
 * @param	num	Number of.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericNCPAP(BYTE num)
{
	m_iCurNumericBlock_NCPAP=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_NCPAP_8, num);
}

/**********************************************************************************************//**
 * @fn	BYTE CConfiguration::getLastNumericDUOPAP()
 * 
 * @brief	Gets the last numeric duopap.
 * 
 * @author	Rainer Kuehner
 * @date	08.02.2017
 * 
 * @return	The last numeric duopap.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric duopap.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericDUOPAP()
{
	return m_iCurNumericBlock_DUOPAP;
}

/**********************************************************************************************//**
 * @fn	void CConfiguration::setLastNumericDUOPAP(BYTE num)
 * 
 * @brief	Sets last numeric duopap.
 * 
 * @author	Rainer Kuehner
 * @date	08.02.2017
 * 
 * @param	num	Number of.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericDUOPAP(BYTE num)
{
	m_iCurNumericBlock_DUOPAP=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_DUOPAP_8, num);
}

/**********************************************************************************************//**
 * @fn	BYTE CConfiguration::getLastNumericTHERAPY()
 * 
 * @brief	Gets the last numeric therapy.
 * 
 * @author	Rainer Kuehner
 * @date	08.02.2017
 * 
 * @return	The last numeric therapy.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric therapy.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericTHERAPY()
{
	return m_iCurNumericBlock_THERAPY;
}

/**********************************************************************************************//**
 * @fn	void CConfiguration::setLastNumericTHERAPY(BYTE num)
 * 
 * @brief	Sets last numeric therapy.
 * 
 * @author	Rainer Kuehner
 * @date	08.02.2017
 * 
 * @param	num	Number of.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericTHERAPY(BYTE num)
{
	m_iCurNumericBlock_THERAPY=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_THERAPY_8, num);
}

/**********************************************************************************************//**
 * @fn	BYTE CConfiguration::getLastNumericFLOWOFFCONV()
 * 
 * @brief	Gets the last numeric flowoffconv.
 * 
 * @author	Rainer Kuehner
 * @date	13.02.2017
 * 
 * @return	The last numeric flowoffconv.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric flowoffconv.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericFLOWOFFCONV()
{
	return m_iCurNumericBlock_FLOWOFFCONV;
}

/**********************************************************************************************//**
 * @fn	void CConfiguration::setLastNumericFLOWOFFCONV(BYTE num)
 * 
 * @brief	Sets last numeric flowoffconv.
 * 
 * @author	Rainer Kuehner
 * @date	13.02.2017
 * 
 * @param	num	Number of.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericFLOWOFFCONV(BYTE num)
{
	m_iCurNumericBlock_FLOWOFFCONV=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_FLOWOFFCONV_8, num);
}

/**********************************************************************************************//**
 * @fn	BYTE CConfiguration::getLastNumericFLOWOFFCPAP()
 * 
 * @brief	Gets the last numeric flowoffcpap.
 * 
 * @author	Rainer Kuehner
 * @date	31.03.2017
 * 
 * @return	The last numeric flowoffcpap.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric flowoffcpap.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericFLOWOFFCPAP()
{
	return m_iCurNumericBlock_FLOWOFFCPAP;
}

/**********************************************************************************************//**
 * @fn	void CConfiguration::setLastNumericFLOWOFFCPAP(BYTE num)
 * 
 * @brief	Sets last numeric flowoffcpap.
 * 
 * @author	Rainer Kuehner
 * @date	31.03.2017
 * 
 * @param	num	Number of.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericFLOWOFFCPAP(BYTE num)
{
	m_iCurNumericBlock_FLOWOFFCPAP=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_FLOWOFFCPAP_8, num);
}

/**********************************************************************************************//**
 * @fn	BYTE CConfiguration::getLastNumericFLOWOFFHFO()
 * 
 * @brief	Gets the last numeric flowoffhfo.
 * 
 * @author	Rainer Kuehner
 * @date	13.02.2017
 * 
 * @return	The last numeric flowoffhfo.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The last numeric flowoffhfo.
 **************************************************************************************************/

BYTE CConfiguration::getLastNumericFLOWOFFHFO()
{
	return m_iCurNumericBlock_FLOWOFFHFO;
}

/**********************************************************************************************//**
 * @fn	void CConfiguration::setLastNumericFLOWOFFHFO(BYTE num)
 * 
 * @brief	Sets last numeric flowoffhfo.
 * 
 * @author	Rainer Kuehner
 * @date	13.02.2017
 * 
 * @param	num	Number of.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	num	Number of.
 **************************************************************************************************/

void CConfiguration::setLastNumericFLOWOFFHFO(BYTE num)
{
	m_iCurNumericBlock_FLOWOFFHFO=num;

	getModel()->getI2C()->WriteConfigByte(NUMBLOCK_FLOWOFFHFO_8, num);
}

/**********************************************************************************************//**
 * Disables the nivtrigger and set trigger to off.
 *
 * \author	Rainer K�hner
 * \date	07.07.2017
 **************************************************************************************************/

void CConfiguration::disableNIVTRIGGER()
{
	SetParaDataTrigger_NCPAP(MAXRANGE_TRIGGER_OFF);
	SetParaDataTrigger_DUOPAP(MAXRANGE_TRIGGER_OFF);

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
}

/**********************************************************************************************//**
 * Gets nivtrigger demo timestamp.
 *
 * \author	Rainer K�hner
 * \date	07.07.2017
 *
 * \return	The nivtrigger demo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetNIVTRIGGERdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_NIVTRIGGER_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_NIVTRIGGER_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_NIVTRIGGER_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** NIVTRIGGERdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_NIVTRIGGER_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_NIVTRIGGER_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_NIVTRIGGER_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets nivtrigger demo timestamp.
 *
 * \author	Rainer K�hner
 * \date	07.07.2017
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetNIVTRIGGERdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_NIVTRIGGER_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_NIVTRIGGER_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_NIVTRIGGER_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set NIVTRIGGERdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * @fn	COleDateTime CConfiguration::GetFOTdemoTimestamp()
 * 
 * @brief	Gets fotdemo timestamp.
 * 
 * @author	Rainer Kuehner
 * @date	09.02.2017
 * 
 * @return	The fotdemo timestamp.
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The fo tdemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetFOTdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_FOT_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_FOT_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_FOT_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** FOTdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_FOT_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_FOT_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_FOT_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}
	
	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets fotdemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetFOTdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_FOT_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_FOT_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_FOT_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set FOTdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets pricodemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The pricodemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetPRICOdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_PRICO_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_PRICO_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_PRICO_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** PRICOdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_PRICO_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_PRICO_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_PRICO_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets pricodemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetPRICOdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_PRICO_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_PRICO_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_PRICO_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set PRICOdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets therapydemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The therapydemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetTHERAPYdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_THERAPY_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_THERAPY_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_THERAPY_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** THERAPYdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_THERAPY_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_THERAPY_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_THERAPY_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets therapydemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetTHERAPYdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_THERAPY_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_THERAPY_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_THERAPY_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set THERAPYdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets trenddemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The trenddemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetTRENDdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_TREND_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_TREND_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_TREND_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** TRENDdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_TREND_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_TREND_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_TREND_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets trenddemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetTRENDdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_TREND_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_TREND_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_TREND_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set TRENDdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets lungrecdemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The lungrecdemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetLUNGRECdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_LUNGREC_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_LUNGREC_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_LUNGREC_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** LUNGRECdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_LUNGREC_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_LUNGREC_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_LUNGREC_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets lungrecdemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetLUNGRECdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_LUNGREC_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_LUNGREC_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_LUNGREC_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set LUNGRECdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets vlimitdemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The vlimitdemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetVLIMITdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_VLIMIT_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_VLIMIT_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_VLIMIT_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** VLIMITdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_VLIMIT_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_VLIMIT_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_VLIMIT_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets vlimitdemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetVLIMITdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_VLIMIT_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_VLIMIT_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_VLIMIT_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set VLIMITdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets vguarantydemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The vguarantydemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetVGUARANTYdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_VGUARANTY_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_VGUARANTY_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_VGUARANTY_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** VGUARANTYdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_VGUARANTY_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_VGUARANTY_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_VGUARANTY_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets vguarantydemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetVGUARANTYdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_VGUARANTY_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_VGUARANTY_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_VGUARANTY_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set VGUARANTYdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets nmodedemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The nmodedemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetNMODEdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_NMODE_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_NMODE_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_NMODE_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** NMODEdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_NMODE_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_NMODE_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_NMODE_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets nmodedemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetNMODEdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{

	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	getModel()->getI2C()->WriteConfigWord(DEMO_NMODE_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_NMODE_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_NMODE_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set NMODEdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Gets hfodemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The hfodemo timestamp.
 **************************************************************************************************/

COleDateTime CConfiguration::GetHFOdemoTimestamp()
{
	WORD iDemoYear=getModel()->getI2C()->ReadConfigWord(DEMO_HFO_YEAR_16);
	WORD iDemoMonth=getModel()->getI2C()->ReadConfigByte(DEMO_HFO_MONTH_8);
	WORD iDemoDay=getModel()->getI2C()->ReadConfigByte(DEMO_HFO_DAY_8);

	COleDateTime dtdemoTimestamp;
	if (iDemoYear<1899 || iDemoMonth>12 || iDemoMonth<1 || iDemoDay>31 || iDemoDay<1)
	{
		dtdemoTimestamp.SetStatus(COleDateTime::null);
		iDemoYear=0;
		iDemoMonth=0;
		iDemoDay=0;
		//theApp.WriteLog(_T("*** HFOdemo license false ***"));
		getModel()->getI2C()->WriteConfigWord(DEMO_HFO_YEAR_16, iDemoYear);
		getModel()->getI2C()->WriteConfigByte(DEMO_HFO_MONTH_8, iDemoMonth);
		getModel()->getI2C()->WriteConfigByte(DEMO_HFO_DAY_8, iDemoDay);
	}
	else
	{
		dtdemoTimestamp.SetDateTime(iDemoYear, iDemoMonth, iDemoDay,0,0,0);
	}

	return dtdemoTimestamp;
}

/**********************************************************************************************//**
 * Sets hfodemo timestamp
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	dateTime	The date time.
 **************************************************************************************************/

void CConfiguration::SetHFOdemoTimestamp(COleDateTime dateTime)
{
	WORD iDemoYear=0;
	WORD iDemoMonth=0;
	WORD iDemoDay=0;

	if(dateTime.GetStatus()==COleDateTime::null)
	{
		
	}
	else if(dateTime.GetStatus()!=COleDateTime::valid)
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	else
	{
		iDemoYear=dateTime.GetYear();
		iDemoMonth=dateTime.GetMonth();
		iDemoDay=dateTime.GetDay();
	}
	
	getModel()->getI2C()->WriteConfigWord(DEMO_HFO_YEAR_16, iDemoYear);
	getModel()->getI2C()->WriteConfigByte(DEMO_HFO_MONTH_8, iDemoMonth);
	getModel()->getI2C()->WriteConfigByte(DEMO_HFO_DAY_8, iDemoDay);

	CString szTxt=_T("");
	szTxt.Format(_T("***Set HFOdemo license:%02d.%02d.%04d"),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear());
	theApp.WriteLog(szTxt);
}

/**********************************************************************************************//**
 * Reads amp cor factor
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 **************************************************************************************************/

void CConfiguration::readAmpCorFactor()
{
	if(CTlsFile::Exists(AMPCORFACTORINI))
	{
		//CStringW csTemp=_T("");
		CString szFactor=_T("");
		CStringW szFreq=_T("");
		BYTE byFreq=5;
		for(int i=0;i<16;i++)
		{
			szFreq.Format(_T("%d"),i+5);
			szFactor=CTlsIniFile::ReadIniStr(_T("FACTOR"), szFreq, _T("1"), AMPCORFACTORINI);
			
			dbBufAmpCorFactor[i] = (double)(_wtoi(szFactor))/1000;

			//csTemp.Format(_T("factor %s %d:%0.3f\r\n"), szFreq,i,dbBufAmpCorFactor[i]);
			//DEBUGMSG(TRUE, (csTemp));
		}

		szFactor=CTlsIniFile::ReadIniStr(_T("CONV_FACTOR"), _T("K"), _T("10000"), AMPCORFACTORINI);
		dbAmpCorFactorCONV_K= (double)(_wtoi(szFactor))/10000;

		szFactor=CTlsIniFile::ReadIniStr(_T("CONV_FACTOR"), _T("J"), _T("10000"), AMPCORFACTORINI);
		dbAmpCorFactorCONV_J= (double)(_wtoi(szFactor))/10000;

		szFactor=CTlsIniFile::ReadIniStr(_T("HFO_FACTOR"), _T("K"), _T("10000"), AMPCORFACTORINI);
		dbAmpCorFactorHFO_K= (double)(_wtoi(szFactor))/10000;

		szFactor=CTlsIniFile::ReadIniStr(_T("HFO_FACTOR"), _T("J"), _T("10000"), AMPCORFACTORINI);
		dbAmpCorFactorHFO_J= (double)(_wtoi(szFactor))/10000;

	}
	else
	{
		for(int i=0;i<16;i++)
		{
			dbBufAmpCorFactor[i] = 1;

			theApp.WriteLog(_T("### ERROR: AMPCORFACTORINI"));
		}
	}
}

/**********************************************************************************************//**
 * Gets amp cor factor
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \param	iFreq	Zero-based index of the frequency.
 *
 * \return	The amp cor factor.
 **************************************************************************************************/

double CConfiguration::getAmpCorFactor(BYTE iFreq)
{
	if(iFreq<5 || iFreq>20)
		iFreq=5;
	
	return dbBufAmpCorFactor[iFreq-5];
}

/**********************************************************************************************//**
 * Gets amp cor factor conventional k
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The amp cor factor conventional k.
 **************************************************************************************************/

double CConfiguration::getAmpCorFactorCONV_K()
{
	return dbAmpCorFactorCONV_K;
}

/**********************************************************************************************//**
 * Gets amp cor factor conventional j
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The amp cor factor conventional j.
 **************************************************************************************************/

double CConfiguration::getAmpCorFactorCONV_J()
{
	return dbAmpCorFactorCONV_J;
}

/**********************************************************************************************//**
 * Gets amp cor factor hfo k
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The amp cor factor hfo k.
 **************************************************************************************************/

double CConfiguration::getAmpCorFactorHFO_K()
{
	return dbAmpCorFactorHFO_K;
}

/**********************************************************************************************//**
 * Gets amp cor factor hfo j
 *
 * \author	Rainer K�hner
 * \date	19.02.2018
 *
 * \return	The amp cor factor hfo j.
 **************************************************************************************************/

double CConfiguration::getAmpCorFactorHFO_J()
{
	return dbAmpCorFactorHFO_J;
}