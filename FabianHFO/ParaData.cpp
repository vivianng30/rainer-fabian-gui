/**********************************************************************************************//**
 * \file	ParaData.cpp.
 *
 * Implements the para data class
 **************************************************************************************************/

#include "StdAfx.h"
#include "ParaData.h"
#include "MVModel.h"
#include "TlsRegistry.h"

CParaData* CParaData::theData=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CParaData class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaData::CParaData(void)
{
	m_pModel = NULL;

	m_data.m_iParaDataITime_TRIGGER=0;
	m_data.m_iParaDataITime_IPPV=0;
	m_data.m_iParaDataITime_NMODE=0;
	m_data.m_iParaDataETime_TRIGGER=0;
	m_data.m_iParaDataETime_IPPV=0;
	m_data.m_iParaDataETime_SIMV=0;
	m_data.m_iParaDataETime_NMODE=0;
	m_data.m_iParaDataBPM_TRIGGER=0;
	m_data.m_iParaDataBPM_IPPV=0;
	m_data.m_iParaDataBPM_SIMV=0;
	m_data.m_iParaDataBPM_NMODE=0;
	m_data.m_iParaDataIFlow_TRIGGER=0;
	m_data.m_iParaDataIFlow_IPPV=0;
	m_data.m_iParaDataHFFlow=0;
	m_data.m_iParaDataRisetime_TRIGGER=0;
	m_data.m_iParaDataRisetime_IPPV=0;
	m_data.m_iParaDataEFlow_TRIGGER=0;
	m_data.m_iParaDataEFlow_IPPV=0;
	m_data.m_iParaDataTherapieFlow=0;
	m_data.m_iParaDataFlowMin=0;
	m_data.m_iParaDataPEEP_IPPV=0;
	m_data.m_iParaDataPEEP_TRIGGER=0;
	m_data.m_iParaDataCPAP=0;
	m_data.m_iParaDataCPAP_NMODE=0;
	m_data.m_iParaDataPInsp_TRIGGER=0;
	m_data.m_iParaDataPInsp_IPPV=0;
	m_data.m_iParaDataPmaxVolG_TRIGGER=0;
	m_data.m_iParaDataPmaxVolG_IPPV=0;
	m_data.m_iParaDataPpsv=0;
	m_data.m_iParaDataPManual_HFO=0;
	m_data.m_iParaDataPManual_CPAP=0;
	m_data.m_iParaDataPManual_NMODE=0;
	m_data.m_iParaDataO2=0;
	m_data.m_iParaDataO2Flush=0;
	m_data.m_iParaDataVLimit_TRIGGER=0;
	m_data.m_bParaDataVLimitOn_TRIGGER=false;
	m_data.m_iParaDataVGarant_TRIGGER=0;
	m_data.m_bParaDataVGarantOn_TRIGGER=false;
	m_data.m_iParaDataVLimit_IPPV=0;
	m_data.m_bParaDataVLimitOn_IPPV=false;
	m_data.m_iParaDataVGarant_IPPV=0;
	m_data.m_bParaDataVGarantOn_IPPV=false;
	m_data.m_iParaDataTrigger_CONV=0;
	m_data.m_iParaDataTrigger_NCPAP=0;
	m_data.m_iParaDataTrigger_DUOPAP=0;
	m_data.m_iParaDataTrigger_CPAP=0;
	m_data.m_iParaDataBackup=0;
	m_data.m_iParaDataHFAMPL=0;
	m_data.m_iParaDataHFAMPLmax=0;
	m_data.m_iParaDataHFFreq=0;
	m_data.m_iParaDataHFPmean=0;
	m_data.m_iParaDataITIME_REC=0;
	m_data.m_iParaDataFREQ_REC=0;
	m_data.m_iParaDataPMEAN_REC=0;
	m_data.m_iParaDataVGarant_HFO=0;
	m_data.m_bParaDataVGarantOn_HFO=false;
	m_data.m_iParaDataIERatio=RIE_1_1;
	m_data.m_iParaDataFOTconv_AMPLITUDE=0;
	m_data.m_iParaDataFOTconv_FREQ=0;
	m_data.m_iParaDataFOTconv_PEEPSTART=0;
	m_data.m_iParaDataFOTconv_PEEPEND=0;
	m_data.m_iParaDataFOTconv_STEPS=0;
	m_data.m_iParaDataFOThfo_AMPLITUDE=0;
	m_data.m_iParaDataFOThfo_PMEANSTART=0;
	m_data.m_iParaDataFOThfo_PMEANEND=0;
	m_data.m_iParaDataFOThfo_FREQ=0;
	m_data.m_iParaDataFOThfo_STEPS=0;
	


	
	m_dataMaxRange_NEONATAL.m_iParaDataITime_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataITime_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataITime_NMODE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataETime_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataETime_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataETime_SIMV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataETime_NMODE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_SIMV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_NMODE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataIFlow_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataIFlow_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataHFFlow=0;
	m_dataMaxRange_NEONATAL.m_iParaDataRisetime_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataRisetime_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataEFlow_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataEFlow_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataTherapieFlow=0;
	m_dataMaxRange_NEONATAL.m_iParaDataFlowMin=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPEEP_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPEEP_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataCPAP=0;
	m_dataMaxRange_NEONATAL.m_iParaDataCPAP_NMODE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPInsp_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPInsp_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPpsv=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPManual_CPAP=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPManual_HFO=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPManual_NMODE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataO2=0;
	m_dataMaxRange_NEONATAL.m_iParaDataO2Flush=0;
	m_dataMaxRange_NEONATAL.m_iParaDataVLimit_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataVGarant_TRIGGER=0;
	m_dataMaxRange_NEONATAL.m_iParaDataVLimit_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataVGarant_IPPV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_CONV=0;
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_NCPAP=0;
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_DUOPAP=0;
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_CPAP=0;
	m_dataMaxRange_NEONATAL.m_iParaDataBackup=0;
	m_dataMaxRange_NEONATAL.m_iParaDataHFAMPL=0;
	m_dataMaxRange_NEONATAL.m_iParaDataHFAMPLmax=0;
	m_dataMaxRange_NEONATAL.m_iParaDataHFFreq=0;
	m_dataMaxRange_NEONATAL.m_iParaDataHFPmean=0;
	m_dataMaxRange_NEONATAL.m_iParaDataITIME_REC=0;
	m_dataMaxRange_NEONATAL.m_iParaDataFREQ_REC=0;
	m_dataMaxRange_NEONATAL.m_iParaDataPMEAN_REC=0;
	m_dataMaxRange_NEONATAL.m_iParaDataVGarant_HFO=0;
	m_dataMaxRange_NEONATAL.m_iParaDataIERatio=RIE_1_1;

	m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_FREQ=0;
	m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_STEPS=0;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_FREQ=0;
	m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_STEPS=0;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_PMEANEND=0;
	
	m_dataMinRange_NEONATAL.m_iParaDataITime_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataITime_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataITime_NMODE=0;
	m_dataMinRange_NEONATAL.m_iParaDataETime_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataETime_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataETime_SIMV=0;
	m_dataMinRange_NEONATAL.m_iParaDataETime_NMODE=0;
	m_dataMinRange_NEONATAL.m_iParaDataBPM_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataBPM_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataBPM_SIMV=0;
	m_dataMinRange_NEONATAL.m_iParaDataBPM_NMODE=0;
	m_dataMinRange_NEONATAL.m_iParaDataIFlow_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataIFlow_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataHFFlow=0;
	m_dataMinRange_NEONATAL.m_iParaDataRisetime_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataRisetime_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataEFlow_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataEFlow_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataTherapieFlow=0;
	m_dataMinRange_NEONATAL.m_iParaDataFlowMin=0;
	m_dataMinRange_NEONATAL.m_iParaDataPEEP_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataPEEP_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataCPAP=0;
	m_dataMinRange_NEONATAL.m_iParaDataCPAP_NMODE=0;
	m_dataMinRange_NEONATAL.m_iParaDataPInsp_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataPInsp_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataPpsv=0;
	m_dataMinRange_NEONATAL.m_iParaDataPManual_CPAP=0;
	m_dataMinRange_NEONATAL.m_iParaDataPManual_HFO=0;
	m_dataMinRange_NEONATAL.m_iParaDataPManual_NMODE=0;
	m_dataMinRange_NEONATAL.m_iParaDataO2=0;
	m_dataMinRange_NEONATAL.m_iParaDataO2Flush=0;
	m_dataMinRange_NEONATAL.m_iParaDataVLimit_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataVGarant_TRIGGER=0;
	m_dataMinRange_NEONATAL.m_iParaDataVLimit_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataVGarant_IPPV=0;
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_CONV=0;
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_NCPAP=0;
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_DUOPAP=0;
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_CPAP=0;
	m_dataMinRange_NEONATAL.m_iParaDataBackup=0;
	m_dataMinRange_NEONATAL.m_iParaDataHFAMPL=0;
	m_dataMinRange_NEONATAL.m_iParaDataHFAMPLmax=0;
	m_dataMinRange_NEONATAL.m_iParaDataHFFreq=0;
	m_dataMinRange_NEONATAL.m_iParaDataHFPmean=0;
	m_dataMinRange_NEONATAL.m_iParaDataITIME_REC=0;
	m_dataMinRange_NEONATAL.m_iParaDataFREQ_REC=0;
	m_dataMinRange_NEONATAL.m_iParaDataPMEAN_REC=0;
	m_dataMinRange_NEONATAL.m_iParaDataVGarant_HFO=0;
	m_dataMinRange_NEONATAL.m_iParaDataIERatio=RIE_1_3;

	m_dataMinRange_NEONATAL.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataMinRange_NEONATAL.m_iParaDataFOTconv_FREQ=0;
	m_dataMinRange_NEONATAL.m_iParaDataFOTconv_STEPS=0;
	//m_dataMinRange_NEONATAL.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataMinRange_NEONATAL.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataMinRange_NEONATAL.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataMinRange_NEONATAL.m_iParaDataFOThfo_FREQ=0;
	m_dataMinRange_NEONATAL.m_iParaDataFOThfo_STEPS=0;
	//m_dataMinRange_NEONATAL.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataMinRange_NEONATAL.m_iParaDataFOThfo_PMEANEND=0;
	
	m_dataMaxRange_PEDIATRIC.m_iParaDataITime_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataITime_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataITime_NMODE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_SIMV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_NMODE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_SIMV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_NMODE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFFlow=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataTherapieFlow=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataFlowMin=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataCPAP=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataCPAP_NMODE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPpsv=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_HFO=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_CPAP=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_NMODE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataO2=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataO2Flush=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataVLimit_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_TRIGGER=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataVLimit_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_IPPV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_CONV=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_NCPAP=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_DUOPAP=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_CPAP=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataBackup=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPL=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPLmax=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFFreq=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFPmean=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataITIME_REC=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataFREQ_REC=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataPMEAN_REC=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_HFO=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataIERatio=RIE_1_1;

	m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_FREQ=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_STEPS=0;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_FREQ=0;
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_STEPS=0;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_PMEANEND=0;
	
	m_dataMinRange_PEDIATRIC.m_iParaDataITime_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataITime_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataITime_NMODE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_SIMV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_NMODE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_SIMV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_NMODE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataHFFlow=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataTherapieFlow=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataFlowMin=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataCPAP=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataCPAP_NMODE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPpsv=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPManual_CPAP=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPManual_HFO=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPManual_NMODE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataO2=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataO2Flush=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataVLimit_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_TRIGGER=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataVLimit_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_IPPV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_CONV=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_NCPAP=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_DUOPAP=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_CPAP=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataBackup=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataHFAMPL=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataHFAMPLmax=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataHFFreq=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataHFPmean=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataITIME_REC=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataFREQ_REC=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataPMEAN_REC=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_HFO=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataIERatio=RIE_1_3;

	m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_FREQ=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_STEPS=0;
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_FREQ=0;
	m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_STEPS=0;
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_PMEANEND=0;
	
	//neonatal
	m_dataKeyMax_NEONATAL.m_iParaDataHFAMPLmax=KEYMAX_NEONATAL_HFAmplVolG;
	m_dataKeyMax_NEONATAL.m_iParaDataPmaxVolG_TRIGGER=KEYMAX_NEONATAL_PmaxVolG;
	m_dataKeyMax_NEONATAL.m_iParaDataPmaxVolG_IPPV=KEYMAX_NEONATAL_PmaxVolG;
	m_dataKeyMax_NEONATAL.m_iParaDataPManual_CPAP=KEYMAX_NEONATAL_PMan;
	m_dataKeyMax_NEONATAL.m_iParaDataPManual_HFO=KEYMAX_NEONATAL_PMan;
	m_dataKeyMax_NEONATAL.m_iParaDataCPAP=KEYMAX_NEONATAL_CPAP;
	m_dataKeyMax_NEONATAL.m_iParaDataCPAP_NMODE=KEYMAX_NEONATAL_CPAP;
	m_dataKeyMax_NEONATAL.m_iParaDataPInsp_TRIGGER=KEYMAX_NEONATAL_PInsp;
	m_dataKeyMax_NEONATAL.m_iParaDataPInsp_IPPV=KEYMAX_NEONATAL_PInsp;
	m_dataKeyMax_NEONATAL.m_iParaDataPpsv=KEYMAX_NEONATAL_PPSV;
	m_dataKeyMax_NEONATAL.m_iParaDataPManual_NMODE=KEYMAX_NEONATAL_PDuo;
	m_dataKeyMax_NEONATAL.m_iParaDataPEEP_TRIGGER=KEYMAX_NEONATAL_PEEP;
	m_dataKeyMax_NEONATAL.m_iParaDataPEEP_IPPV=KEYMAX_NEONATAL_PEEP;
	m_dataKeyMax_NEONATAL.m_iParaDataVGarant_TRIGGER=KEYMAX_NEONATAL_VGarant;
	m_dataKeyMax_NEONATAL.m_iParaDataVGarant_IPPV=KEYMAX_NEONATAL_VGarant;
	m_dataKeyMax_NEONATAL.m_iParaDataVGarant_HFO=KEYMAX_NEONATAL_VGarant;
	m_dataKeyMax_NEONATAL.m_iParaDataVLimit_TRIGGER=KEYMAX_NEONATAL_VLimit;
	m_dataKeyMax_NEONATAL.m_iParaDataVLimit_IPPV=KEYMAX_NEONATAL_VLimit;
	m_dataKeyMax_NEONATAL.m_iParaDataHFPmean=KEYMAX_NEONATAL_PMitt;
	m_dataKeyMax_NEONATAL.m_iParaDataHFAMPL=KEYMAX_NEONATAL_HFAmpl;
	m_dataKeyMax_NEONATAL.m_iParaDataPMEAN_REC=KEYMAX_NEONATAL_PMEAN_REC;
	m_dataKeyMax_NEONATAL.m_iParaDataITIME_REC=KEYMAX_NEONATAL_ITIME_REC;
	m_dataKeyMax_NEONATAL.m_iParaDataFREQ_REC=KEYMAX_NEONATAL_FREQ_REC;
	m_dataKeyMax_NEONATAL.m_iParaDataHFFreq=KEYMAX_NEONATAL_HFFreq;
	m_dataKeyMax_NEONATAL.m_iParaDataIERatio=RIE_1_1;
	m_dataKeyMax_NEONATAL.m_iParaDataTherapieFlow=KEYMAX_NEONATAL_FLOWTHERAPIE;

	m_dataKeyMax_NEONATAL.m_iParaDataIFlow_TRIGGER=KEYMAX_NEONATAL_IFlow;
	m_dataKeyMax_NEONATAL.m_iParaDataIFlow_IPPV=KEYMAX_NEONATAL_IFlow;
	m_dataKeyMax_NEONATAL.m_iParaDataEFlow_TRIGGER=KEYMAX_NEONATAL_EFlow;
	m_dataKeyMax_NEONATAL.m_iParaDataEFlow_IPPV=KEYMAX_NEONATAL_EFlow;

	m_dataKeyMax_NEONATAL.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataKeyMax_NEONATAL.m_iParaDataFOTconv_FREQ=0;
	m_dataKeyMax_NEONATAL.m_iParaDataFOTconv_STEPS=0;
	//m_dataKeyMax_NEONATAL.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataKeyMax_NEONATAL.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataKeyMax_NEONATAL.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataKeyMax_NEONATAL.m_iParaDataFOThfo_FREQ=0;
	m_dataKeyMax_NEONATAL.m_iParaDataFOThfo_STEPS=0;
	//m_dataKeyMax_NEONATAL.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataKeyMax_NEONATAL.m_iParaDataFOThfo_PMEANEND=0;
	
	//pediatric
	m_dataKeyMax_PEDIATRIC.m_iParaDataHFAMPLmax=KEYMAX_PEDIATRIC_HFAmplVolG;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPmaxVolG_IPPV=KEYMAX_PEDIATRIC_PmaxVolG;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER=KEYMAX_PEDIATRIC_PmaxVolG;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPManual_CPAP=KEYMAX_PEDIATRIC_PMan;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPManual_HFO=KEYMAX_PEDIATRIC_PMan;
	m_dataKeyMax_PEDIATRIC.m_iParaDataCPAP=KEYMAX_PEDIATRIC_CPAP;
	m_dataKeyMax_PEDIATRIC.m_iParaDataCPAP_NMODE=KEYMAX_PEDIATRIC_CPAP;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPInsp_IPPV=KEYMAX_PEDIATRIC_PInsp;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPInsp_TRIGGER=KEYMAX_PEDIATRIC_PInsp;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPpsv=KEYMAX_PEDIATRIC_PPSV;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPManual_NMODE=KEYMAX_PEDIATRIC_PDuo;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPEEP_IPPV=KEYMAX_PEDIATRIC_PEEP;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPEEP_TRIGGER=KEYMAX_PEDIATRIC_PEEP;
	m_dataKeyMax_PEDIATRIC.m_iParaDataVGarant_TRIGGER=KEYMAX_PEDIATRIC_VGarant;
	m_dataKeyMax_PEDIATRIC.m_iParaDataVGarant_IPPV=KEYMAX_PEDIATRIC_VGarant;
	m_dataKeyMax_PEDIATRIC.m_iParaDataVGarant_HFO=KEYMAX_PEDIATRIC_VGarant;
	m_dataKeyMax_PEDIATRIC.m_iParaDataVLimit_TRIGGER=KEYMAX_PEDIATRIC_VLimit;
	m_dataKeyMax_PEDIATRIC.m_iParaDataVLimit_IPPV=KEYMAX_PEDIATRIC_VLimit;
	m_dataKeyMax_PEDIATRIC.m_iParaDataHFPmean=KEYMAX_PEDIATRIC_PMitt;
	m_dataKeyMax_PEDIATRIC.m_iParaDataHFAMPL=KEYMAX_PEDIATRIC_HFAmpl;
	m_dataKeyMax_PEDIATRIC.m_iParaDataPMEAN_REC=KEYMAX_PEDIATRIC_PMEAN_REC;
	m_dataKeyMax_PEDIATRIC.m_iParaDataITIME_REC=KEYMAX_PEDIATRIC_ITIME_REC;
	m_dataKeyMax_PEDIATRIC.m_iParaDataFREQ_REC=KEYMAX_PEDIATRIC_FREQ_REC;
	m_dataKeyMax_PEDIATRIC.m_iParaDataHFFreq=KEYMAX_PEDIATRIC_HFFreq;
	m_dataKeyMax_PEDIATRIC.m_iParaDataIERatio=RIE_1_1;
	m_dataKeyMax_PEDIATRIC.m_iParaDataTherapieFlow=KEYMAX_PEDIATRIC_FLOWTHERAPIE;

	m_dataKeyMax_PEDIATRIC.m_iParaDataIFlow_TRIGGER=KEYMAX_PEDIATRIC_IFlow;
	m_dataKeyMax_PEDIATRIC.m_iParaDataIFlow_IPPV=KEYMAX_PEDIATRIC_IFlow;
	m_dataKeyMax_PEDIATRIC.m_iParaDataEFlow_TRIGGER=KEYMAX_PEDIATRIC_EFlow;
	m_dataKeyMax_PEDIATRIC.m_iParaDataEFlow_IPPV=KEYMAX_PEDIATRIC_EFlow;

	m_dataKeyMax_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataKeyMax_PEDIATRIC.m_iParaDataFOTconv_FREQ=0;
	m_dataKeyMax_PEDIATRIC.m_iParaDataFOTconv_STEPS=0;
	//m_dataKeyMax_PEDIATRIC.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataKeyMax_PEDIATRIC.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataKeyMax_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataKeyMax_PEDIATRIC.m_iParaDataFOThfo_FREQ=0;
	m_dataKeyMax_PEDIATRIC.m_iParaDataFOThfo_STEPS=0;
	//m_dataKeyMax_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataKeyMax_PEDIATRIC.m_iParaDataFOThfo_PMEANEND=0;
	
	//neonatal
	m_dataKeyMin_NEONATAL.m_iParaDataHFAMPLmax=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPmaxVolG_TRIGGER=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPmaxVolG_IPPV=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPManual_CPAP=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPManual_HFO=0;
	m_dataKeyMin_NEONATAL.m_iParaDataCPAP=0;
	m_dataKeyMin_NEONATAL.m_iParaDataCPAP_NMODE=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPInsp_TRIGGER=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPInsp_IPPV=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPpsv=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPManual_NMODE=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPEEP_TRIGGER=KEYMIN_NEONATAL_PEEP;
	m_dataKeyMin_NEONATAL.m_iParaDataPEEP_IPPV=KEYMIN_NEONATAL_PEEP;
	m_dataKeyMin_NEONATAL.m_iParaDataHFPmean=0;
	m_dataKeyMin_NEONATAL.m_iParaDataITIME_REC=0;
	m_dataKeyMin_NEONATAL.m_iParaDataFREQ_REC=0;
	m_dataKeyMin_NEONATAL.m_iParaDataPMEAN_REC=0;
	m_dataKeyMin_NEONATAL.m_iParaDataHFAMPL=0;
	m_dataKeyMin_NEONATAL.m_iParaDataHFFreq=0;
	m_dataKeyMin_NEONATAL.m_iParaDataIERatio=RIE_1_3;
	m_dataKeyMin_NEONATAL.m_iParaDataVGarant_TRIGGER=0;
	m_dataKeyMin_NEONATAL.m_iParaDataVGarant_IPPV=0;
	m_dataKeyMin_NEONATAL.m_iParaDataVGarant_HFO=0;
	m_dataKeyMin_NEONATAL.m_iParaDataVLimit_TRIGGER=0;
	m_dataKeyMin_NEONATAL.m_iParaDataVLimit_IPPV=0;
	m_dataKeyMin_NEONATAL.m_iParaDataTherapieFlow=0;

	m_dataKeyMin_NEONATAL.m_iParaDataIFlow_TRIGGER=0;
	m_dataKeyMin_NEONATAL.m_iParaDataIFlow_IPPV=0;
	m_dataKeyMin_NEONATAL.m_iParaDataEFlow_TRIGGER=0;
	m_dataKeyMin_NEONATAL.m_iParaDataEFlow_IPPV=0;

	m_dataKeyMin_NEONATAL.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataKeyMin_NEONATAL.m_iParaDataFOTconv_FREQ=0;
	m_dataKeyMin_NEONATAL.m_iParaDataFOTconv_STEPS=0;
	//m_dataKeyMin_NEONATAL.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataKeyMin_NEONATAL.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataKeyMin_NEONATAL.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataKeyMin_NEONATAL.m_iParaDataFOThfo_FREQ=0;
	m_dataKeyMin_NEONATAL.m_iParaDataFOThfo_STEPS=0;
	//m_dataKeyMin_NEONATAL.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataKeyMin_NEONATAL.m_iParaDataFOThfo_PMEANEND=0;
	
	//pediatric
	m_dataKeyMin_PEDIATRIC.m_iParaDataHFAMPLmax=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPmaxVolG_IPPV=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPManual_CPAP=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPManual_HFO=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataCPAP=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataCPAP_NMODE=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPInsp_IPPV=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPInsp_TRIGGER=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPpsv=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPManual_NMODE=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPEEP_IPPV=KEYMIN_PEDIATRIC_PEEP;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPEEP_TRIGGER=KEYMIN_PEDIATRIC_PEEP;
	m_dataKeyMin_PEDIATRIC.m_iParaDataHFPmean=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataITIME_REC=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataFREQ_REC=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataPMEAN_REC=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataHFAMPL=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataHFFreq=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataIERatio=RIE_1_3;
	m_dataKeyMin_PEDIATRIC.m_iParaDataVGarant_TRIGGER=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataVGarant_IPPV=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataVGarant_HFO=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataVLimit_TRIGGER=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataVLimit_IPPV=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataTherapieFlow=0;

	m_dataKeyMin_PEDIATRIC.m_iParaDataIFlow_TRIGGER=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataIFlow_IPPV=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataEFlow_TRIGGER=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataEFlow_IPPV=0;

	m_dataKeyMin_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataFOTconv_FREQ=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataFOTconv_STEPS=0;
	//m_dataKeyMin_PEDIATRIC.m_iParaDataFOTconv_PEEPSTART=0;
	//m_dataKeyMin_PEDIATRIC.m_iParaDataFOTconv_PEEPEND=0;
	
	m_dataKeyMin_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataFOThfo_FREQ=0;
	m_dataKeyMin_PEDIATRIC.m_iParaDataFOThfo_STEPS=0;
	//m_dataKeyMin_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART=0;
	//m_dataKeyMin_PEDIATRIC.m_iParaDataFOThfo_PMEANEND=0;
	
	m_byFOTAmp=0;
	m_byFOTFreq=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaData class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaData::~CParaData(void)
{
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CParaData::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CParaData* CParaData::GetInstance()
{
	if(theData == 0)
	{
		theData = new CParaData;
	}
	return theData;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CParaData::DestroyInstance()
{
	if(theData != NULL)
	{
		delete theData;
		theData = NULL;
	}
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CParaData::Init()
{
	m_data.m_iParaDataITime_TRIGGER=getModel()->getCONFIG()->GetParaDataITime_TRIGGER();
	m_data.m_iParaDataITime_IPPV=getModel()->getCONFIG()->GetParaDataITime_IPPV();
	m_data.m_iParaDataITime_NMODE=getModel()->getCONFIG()->GetParaDataITime_NMODE();
	m_data.m_iParaDataETime_TRIGGER=getModel()->getCONFIG()->GetParaDataETIME_TRIGGER();
	m_data.m_iParaDataETime_IPPV=getModel()->getCONFIG()->GetParaDataETIME_IPPV();
	m_data.m_iParaDataETime_NMODE=getModel()->getCONFIG()->GetParaDataETIME_NMODE();
	m_data.m_iParaDataBPM_TRIGGER=getModel()->getCONFIG()->GetParaDataBPM_TRIGGER();
	m_data.m_iParaDataBPM_IPPV=getModel()->getCONFIG()->GetParaDataBPM_IPPV();
	m_data.m_iParaDataBPM_NMODE=getModel()->getCONFIG()->GetParaDataBPM_NMODE();
	m_data.m_iParaDataIFlow_TRIGGER=getModel()->getCONFIG()->GetParaDataIFlow_TRIGGER();
	m_data.m_iParaDataIFlow_IPPV=getModel()->getCONFIG()->GetParaDataIFlow_IPPV();
	m_data.m_iParaDataHFFlow=getModel()->getCONFIG()->GetParaDataHFFlow();
	m_data.m_iParaDataRisetime_IPPV=getModel()->getCONFIG()->GetParaDataRisetime_IPPV();
	m_data.m_iParaDataRisetime_TRIGGER=getModel()->getCONFIG()->GetParaDataRisetime_TRIGGER();
	m_data.m_iParaDataEFlow_IPPV=getModel()->getCONFIG()->GetParaDataEFlow_IPPV();
	m_data.m_iParaDataEFlow_TRIGGER=getModel()->getCONFIG()->GetParaDataEFlow_TRIGGER();
	m_data.m_iParaDataTherapieFlow=getModel()->getCONFIG()->GetParaDataTherapieFlow();
	m_data.m_iParaDataFlowMin=getModel()->getCONFIG()->GetParaDataFlowMin();
	m_data.m_iParaDataPEEP_TRIGGER=getModel()->getCONFIG()->GetParaDataPEEP_TRIGGER();
	m_data.m_iParaDataPEEP_IPPV=getModel()->getCONFIG()->GetParaDataPEEP_IPPV();
	//m_data.m_iParaDataCPAP=getModel()->getCONFIG()->GetParaDataCPAP();
	m_data.m_iParaDataCPAP_NMODE=getModel()->getCONFIG()->GetParaDataCPAP_NMODE();
	m_data.m_iParaDataPInsp_IPPV=getModel()->getCONFIG()->GetParaDataPInsp_IPPV();
	m_data.m_iParaDataPInsp_TRIGGER=getModel()->getCONFIG()->GetParaDataPInsp_TRIGGER();
	m_data.m_iParaDataPmaxVolG_IPPV=getModel()->getCONFIG()->GetParaDataPmaxVolG_IPPV();
	m_data.m_iParaDataPmaxVolG_TRIGGER=getModel()->getCONFIG()->GetParaDataPmaxVolG_TRIGGER();
	m_data.m_iParaDataPpsv=getModel()->getCONFIG()->GetParaDataPpsv();
	m_data.m_iParaDataPManual_HFO=getModel()->getCONFIG()->GetParaDataPManual_HFO();
	//m_data.m_iParaDataPManual_CPAP=getModel()->getCONFIG()->GetParaDataPManual_CPAP();
	m_data.m_iParaDataPManual_NMODE=getModel()->getCONFIG()->GetParaDataPManual_NMODE();
	m_data.m_iParaDataO2=getModel()->getCONFIG()->GetParaDataO2();
	m_data.m_iParaDataO2Flush=getModel()->getCONFIG()->GetParaDataO2Flush();
	m_data.m_iParaDataVLimit_TRIGGER=getModel()->getCONFIG()->GetParaDataVLimit_TRIGGER();
	m_data.m_bParaDataVLimitOn_TRIGGER=getModel()->getCONFIG()->GetParaDataVLimitState_TRIGGER();
	m_data.m_iParaDataVLimit_IPPV=getModel()->getCONFIG()->GetParaDataVLimit_IPPV();
	m_data.m_bParaDataVLimitOn_IPPV=getModel()->getCONFIG()->GetParaDataVLimitState_IPPV();
	m_data.m_iParaDataTrigger_CONV=getModel()->getCONFIG()->GetParaDataTrigger_CONV();
	m_data.m_iParaDataTrigger_CPAP=getModel()->getCONFIG()->GetParaDataTrigger_CPAP();
	m_data.m_iParaDataTrigger_DUOPAP=getModel()->getCONFIG()->GetParaDataTrigger_DUOPAP();
	m_data.m_iParaDataTrigger_NCPAP=getModel()->getCONFIG()->GetParaDataTrigger_NCPAP();

	m_data.m_iParaDataBackup=getModel()->getCONFIG()->GetParaDataBackup();
	m_data.m_iParaDataHFAMPL=getModel()->getCONFIG()->GetParaDataHFAmpl();
	m_data.m_iParaDataHFAMPLmax=getModel()->getCONFIG()->GetParaDataHFAmplmax();
	m_data.m_iParaDataHFFreq=getModel()->getCONFIG()->GetParaDataHFFreq();
	m_data.m_iParaDataHFPmean=getModel()->getCONFIG()->GetParaDataHFPmean();
	//check against Pmean (Pman>=Pmean+1mbar
	//if(getModel()->getDATAHANDLER()->isHFOManBreathAvailable())//PMAN1
	//{
	//	if(m_data.m_iParaDataPManual<m_data.m_iParaDataHFPmean+10)
	//	{
	//		m_data.m_iParaDataPManual=m_data.m_iParaDataHFPmean+10;
	//		getModel()->getCONFIG()->SetParaDataPManual(m_data.m_iParaDataPManual,false);
	//	}
	//}
	m_data.m_iParaDataIERatio=getModel()->getCONFIG()->GetParaDataHFIERatio();
	m_data.m_iParaDataITIME_REC=getModel()->getCONFIG()->GetParaDataITimeRec();
	m_data.m_iParaDataFREQ_REC=getModel()->getCONFIG()->GetParaDataFREQ_REC();
	m_data.m_iParaDataPMEAN_REC=getModel()->getCONFIG()->GetParaDataHFPMeanRec();

	m_data.m_iParaDataVGarant_TRIGGER=getModel()->getCONFIG()->GetParaDataVGarant_TRIGGER();
	m_data.m_iParaDataVGarant_IPPV=getModel()->getCONFIG()->GetParaDataVGarant_IPPV();
	m_data.m_iParaDataVGarant_HFO=getModel()->getCONFIG()->GetParaDataHFVGarant();


	m_data.m_bParaDataVGarantOn_IPPV=getModel()->getCONFIG()->GetParaDataVGarantState_IPPV();
	if(m_data.m_bParaDataVGarantOn_IPPV==true)
	{
		m_data.m_bParaDataVGarantOn_IPPV=false;
		SetVGarantPara_IPPV(m_data.m_iParaDataVGarant_IPPV,true,true);
	}

	m_data.m_bParaDataVGarantOn_TRIGGER=getModel()->getCONFIG()->GetParaDataVGarantState_TRIGGER();
	if(m_data.m_bParaDataVGarantOn_TRIGGER==true)
	{
		m_data.m_bParaDataVGarantOn_TRIGGER=false;
		SetVGarantPara_TRIGGER(m_data.m_iParaDataVGarant_TRIGGER,true,true);
	}

	m_data.m_bParaDataVGarantOn_HFO=getModel()->getCONFIG()->GetParaDataHFVGarantState();
	if(m_data.m_bParaDataVGarantOn_HFO==true)
	{
		m_data.m_bParaDataVGarantOn_HFO=false;
		SetHFVGarantPara(m_data.m_iParaDataVGarant_HFO, true, true);
	}

	m_data.m_iParaDataFOTconv_AMPLITUDE=getModel()->getCONFIG()->getFOTconv_AMPLITUDE();
	m_data.m_iParaDataFOTconv_FREQ=getModel()->getCONFIG()->getFOTconv_FREQ();
	m_data.m_iParaDataFOTconv_STEPS=getModel()->getCONFIG()->getFOTconv_STEPS();
	m_data.m_iParaDataFOTconv_PEEPSTART=getModel()->getCONFIG()->getFOTconv_PEEPSTART();
	m_data.m_iParaDataFOTconv_PEEPEND=getModel()->getCONFIG()->getFOTconv_PEEPEND();
	
	m_data.m_iParaDataFOThfo_AMPLITUDE=getModel()->getCONFIG()->getFOThfo_AMPLITUDE();
	m_data.m_iParaDataFOThfo_PMEANSTART=getModel()->getCONFIG()->getFOThfo_PMEANSTART();
	m_data.m_iParaDataFOThfo_PMEANEND=getModel()->getCONFIG()->getFOThfo_PMEANEND();
	m_data.m_iParaDataFOThfo_FREQ=getModel()->getCONFIG()->getFOThfo_FREQ();
	m_data.m_iParaDataFOThfo_STEPS=getModel()->getCONFIG()->getFOThfo_STEPS();
	
	
	

	//WORD FOTAmp=getModel()->getCONFIG()->getFOTconv_AMPLITUDE();
	switch(m_data.m_iParaDataFOTconv_AMPLITUDE)
	{
	/*case 0:
		{
			m_byFOTAmp=0;
		}
		break;*/
	case 25:
		{
			m_byFOTAmp=1;
		}
		break;
	case 50:
		{
			m_byFOTAmp=2;
		}
		break;
	case 75:
		{
			m_byFOTAmp=3;
		}
		break;
	default:
		{
			m_byFOTAmp=0;
		}
		break;
	}
	
	//WORD FOTFreq=getModel()->getCONFIG()->getFOTconv_FREQ();
	switch(m_data.m_iParaDataFOTconv_FREQ)
	{
	case 5:
		{
			m_byFOTFreq=0;
		}
		break;
	case 10:
		{
			m_byFOTFreq=4;
		}
		break;
	case 15:
		{
			m_byFOTFreq=8;
		}
		break;
	default:
		{
			m_byFOTFreq=0;
		}
		break;
	}

	loadLimits();
	checkIErelation();
}

/**********************************************************************************************//**
 * Loads the limits
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CParaData::loadLimits()
{
	CTlsRegistry regLimit(_T("HKCU\\Software\\FabianHFO\\WorkState\\Limits"),true);	

	m_dataMaxRange_NEONATAL.m_iParaDataITime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ITIME_TRIGGER"), MAXRANGE_NEO_ITIME_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataITime_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ITIME_IPPV"), MAXRANGE_NEO_ITIME_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataITime_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ITIME_NMODE"), MAXRANGE_NEO_ITIME_NMODE);
	m_dataMaxRange_NEONATAL.m_iParaDataETime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ETIME_TRIGGER"), MAXRANGE_NEO_ETIME_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataETime_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ETIME_IPPV"), MAXRANGE_NEO_ETIME_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataETime_SIMV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ETIME_SIMV"), MAXRANGE_NEO_ETIME_SIMV);
	m_dataMaxRange_NEONATAL.m_iParaDataETime_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ETIME_NMODE"), MAXRANGE_NEO_ETIME_NMODE);
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BPM_TRIGGER"), MAXRANGE_NEO_BPM_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BPM_IPPV"), MAXRANGE_NEO_BPM_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_SIMV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BPM_SIMV"), MAXRANGE_NEO_BPM_SIMV);
	m_dataMaxRange_NEONATAL.m_iParaDataBPM_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BPM_NMODE"), MAXRANGE_NEO_BPM_NMODE);
	m_dataMaxRange_NEONATAL.m_iParaDataIFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_IFLOW_TRIGGER"), MAXRANGE_NEO_IFLOW_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataIFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_IFLOW_IPPV"), MAXRANGE_NEO_IFLOW_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataHFFlow=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFFLOW"), MAXRANGE_NEO_HFFLOW);
	m_dataMaxRange_NEONATAL.m_iParaDataRisetime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_RISETIME_TRIGGER"), MAXRANGE_NEO_RISETIME_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataRisetime_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_RISETIME_IPPV"), MAXRANGE_NEO_RISETIME_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataEFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_EFLOW_TRIGGER"), MAXRANGE_NEO_EFLOW_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataEFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_EFLOW_IPPV"), MAXRANGE_NEO_EFLOW_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataTherapieFlow=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_THERAPYFLOW"), MAXRANGE_NEO_THERAPYFLOW);
	m_dataMaxRange_NEONATAL.m_iParaDataFlowMin=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FLOWMIN"), MAXRANGE_NEO_FLOWMIN);
	m_dataMaxRange_NEONATAL.m_iParaDataPEEP_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PEEP_TRIGGER"), MAXRANGE_NEO_PEEP_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataPEEP_IPPV=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PEEP_IPPV"), MAXRANGE_NEO_PEEP_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataCPAP=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_CPAP"), MAXRANGE_NEO_CPAP);
	m_dataMaxRange_NEONATAL.m_iParaDataCPAP_NMODE=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_CPAP_NMODE"), MAXRANGE_NEO_CPAP_NMODE);
	m_dataMaxRange_NEONATAL.m_iParaDataPInsp_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PINSP_TRIGGER"), MAXRANGE_NEO_PINSP_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataPInsp_IPPV=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PINSP_IPPV"), MAXRANGE_NEO_PINSP_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PMAXVOLG_TRIGGER"), MAXRANGE_NEO_PMAXVOLG_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_IPPV=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PMAXVOLG_IPPV"), MAXRANGE_NEO_PMAXVOLG_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataPpsv=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PPSV"), MAXRANGE_NEO_PPSV);
	m_dataMaxRange_NEONATAL.m_iParaDataPManual_HFO=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PMANUAL"), MAXRANGE_NEO_PMANUAL);
	m_dataMaxRange_NEONATAL.m_iParaDataPManual_CPAP=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PMANUAL"), MAXRANGE_NEO_PMANUAL);
	m_dataMaxRange_NEONATAL.m_iParaDataPManual_NMODE=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PMANUAL_NMODE"), MAXRANGE_NEO_PMANUAL_NMODE);
	m_dataMaxRange_NEONATAL.m_iParaDataO2=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_O2"), MAXRANGE_NEO_O2);
	m_dataMaxRange_NEONATAL.m_iParaDataO2Flush=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_O2FLUSH"), MAXRANGE_NEO_O2FLUSH);
	m_dataMaxRange_NEONATAL.m_iParaDataVLimit_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_VLIMIT_TRIGGER"), MAXRANGE_NEO_VLIMIT_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataVLimit_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_VLIMIT_IPPV"), MAXRANGE_NEO_VLIMIT_IPPV);
	m_dataMaxRange_NEONATAL.m_iParaDataVGarant_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_VGARANT_TRIGGER"), MAXRANGE_NEO_VGARANT_TRIGGER);
	m_dataMaxRange_NEONATAL.m_iParaDataVGarant_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_VGARANT_IPPV"), MAXRANGE_NEO_VGARANT_IPPV);

	/*regLimit.WriteDWORD(_T("MAXRANGE_NEO_TRIGGER_CONV"), MAXRANGE_NEO_TRIGGER_CONV);
	regLimit.WriteDWORD(_T("MAXRANGE_NEO_TRIGGER_CPAP"), MAXRANGE_NEO_TRIGGER_CPAP);
	regLimit.WriteDWORD(_T("MAXRANGE_NEO_TRIGGER_DUOPAP"), MAXRANGE_TRIGGER_OFF);
	regLimit.WriteDWORD(_T("MAXRANGE_NEO_TRIGGER_NCPAP"), MAXRANGE_TRIGGER_OFF);*/
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_CONV=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_TRIGGER_CONV"), MAXRANGE_NEO_TRIGGER_CONV);
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_CPAP=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_TRIGGER_CPAP"), MAXRANGE_NEO_TRIGGER_CPAP);
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_DUOPAP=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_TRIGGER_DUOPAP"), MAXRANGE_TRIGGER_OFF);
	m_dataMaxRange_NEONATAL.m_iParaDataTrigger_NCPAP=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_TRIGGER_NCPAP"), MAXRANGE_TRIGGER_OFF);

	m_dataMaxRange_NEONATAL.m_iParaDataBackup=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_BACKUP"), MAXRANGE_NEO_BACKUP);
	if(getModel()->getDATAHANDLER()->IsAccuSupply()==true)
	{
		m_dataMaxRange_NEONATAL.m_iParaDataHFAMPL=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_ACCU_HFAMPL"), MAXRANGE_ACCU_HFAMPL);
		m_dataMaxRange_NEONATAL.m_iParaDataHFAMPLmax=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_ACCU_HFAMPLVOLG"), MAXRANGE_ACCU_HFAMPLVOLG);
	}
	else
	{
		m_dataMaxRange_NEONATAL.m_iParaDataHFAMPL=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFAMPL"), MAXRANGE_NEO_HFAMPL);
		m_dataMaxRange_NEONATAL.m_iParaDataHFAMPLmax=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFAMPLVOLG"), MAXRANGE_NEO_HFAMPLVOLG);
	}
	m_dataMaxRange_NEONATAL.m_iParaDataHFPmean=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFPMEAN"), MAXRANGE_NEO_HFPMEAN);
	m_dataMaxRange_NEONATAL.m_iParaDataHFFreq=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFFREQ"), MAXRANGE_NEO_HFFREQ);
	m_dataMaxRange_NEONATAL.m_iParaDataVGarant_HFO=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFVGARANT"), MAXRANGE_NEO_HFVGARANT);
	m_dataMaxRange_NEONATAL.m_iParaDataIERatio=(eRatioIE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_IERATIO"), MAXRANGE_NEO_IERATIO);
	m_dataMaxRange_NEONATAL.m_iParaDataITIME_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_ITIME_REC"), MAXRANGE_NEO_ITIME_REC);
	m_dataMaxRange_NEONATAL.m_iParaDataFREQ_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FREQ_REC"), MAXRANGE_NEO_FREQ_REC);
	m_dataMaxRange_NEONATAL.m_iParaDataPMEAN_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_PMEAN_REC"), MAXRANGE_NEO_PMEAN_REC);

	m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FOTCONV_AMPLITUDE"), MAXRANGE_NEO_FOTCONV_AMPLITUDE);
	m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_FREQ=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FOTCONV_FREQ"), MAXRANGE_NEO_FOTCONV_FREQ);
	m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_STEPS=MAXSIZE_FOT_STEPS;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_PEEPSTART=xxx;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_PEEPEND=xxx;

	m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FOTHFO_AMPLITUDE"), MAXRANGE_NEO_FOTHFO_AMPLITUDE);
	m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_FREQ=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FOTHFO_FREQ"), MAXRANGE_NEO_FOTHFO_FREQ);
	m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_STEPS=MAXSIZE_FOT_STEPS;
	//m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_PMEANSTART=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FOTHFO_PMEANSTART"), MAXRANGE_NEO_FOTHFO_PMEANSTART);
	//m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_PMEANEND=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_NEO_FOTHFO_PMEANEND"), MAXRANGE_NEO_FOTHFO_PMEANEND);
	
	m_dataMinRange_NEONATAL.m_iParaDataITime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_TRIGGER"), MINRANGE_NEO_ITIME_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataITime_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_IPPV"), MINRANGE_NEO_ITIME_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataITime_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_NMODE"), MINRANGE_NEO_ITIME_NMODE);
	m_dataMinRange_NEONATAL.m_iParaDataETime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_TRIGGER"), MINRANGE_NEO_ETIME_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataETime_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_IPPV"), MINRANGE_NEO_ETIME_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataETime_SIMV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_SIMV"), MINRANGE_NEO_ETIME_SIMV);
	m_dataMinRange_NEONATAL.m_iParaDataETime_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ETIME_NMODE"), MINRANGE_NEO_ETIME_NMODE);
	m_dataMinRange_NEONATAL.m_iParaDataBPM_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_BPM_TRIGGER"), MINRANGE_NEO_BPM_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataBPM_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_BPM_IPPV"), MINRANGE_NEO_BPM_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataBPM_SIMV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_BPM_SIMV"), MINRANGE_NEO_BPM_SIMV);
	m_dataMinRange_NEONATAL.m_iParaDataBPM_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_BPM_NMODE"), MINRANGE_NEO_BPM_NMODE);
	m_dataMinRange_NEONATAL.m_iParaDataIFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_IFLOW_TRIGGER"), MINRANGE_NEO_IFLOW_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataIFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_IFLOW_IPPV"), MINRANGE_NEO_IFLOW_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataHFFlow=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFFLOW"), MINRANGE_NEO_HFFLOW);
	m_dataMinRange_NEONATAL.m_iParaDataRisetime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_RISETIME_TRIGGER"), MINRANGE_NEO_RISETIME_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataRisetime_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_RISETIME_IPPV"), MINRANGE_NEO_RISETIME_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataEFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_EFLOW_TRIGGER"), MINRANGE_NEO_EFLOW_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataEFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_EFLOW_IPPV"), MINRANGE_NEO_EFLOW_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataTherapieFlow=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_THERAPYFLOW"), MINRANGE_NEO_THERAPYFLOW);
	m_dataMinRange_NEONATAL.m_iParaDataFlowMin=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FLOWMIN"), MINRANGE_NEO_FLOWMIN);
	m_dataMinRange_NEONATAL.m_iParaDataPEEP_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PEEP_TRIGGER"), MINRANGE_NEO_PEEP_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataPEEP_IPPV=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PEEP_IPPV"), MINRANGE_NEO_PEEP_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataPEEP_RISETIME=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PEEP_RISETIME"), MINRANGE_NEO_PEEP_RISETIME);
	m_dataMinRange_NEONATAL.m_iParaDataCPAP=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_CPAP"), MINRANGE_NEO_CPAP);
	m_dataMinRange_NEONATAL.m_iParaDataCPAP_NMODE=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_CPAP_NMODE"), MINRANGE_NEO_CPAP_NMODE);
	m_dataMinRange_NEONATAL.m_iParaDataPInsp_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PINSP_TRIGGER"), MINRANGE_NEO_PINSP_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataPInsp_IPPV=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PINSP_IPPV"), MINRANGE_NEO_PINSP_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PMAXVOLG_TRIGGER"), MINRANGE_NEO_PMAXVOLG_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_IPPV=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PMAXVOLG_IPPV"), MINRANGE_NEO_PMAXVOLG_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataPpsv=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PPSV"), MINRANGE_NEO_PPSV);
	m_dataMinRange_NEONATAL.m_iParaDataPManual_CPAP=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PMANUAL"), MINRANGE_NEO_PMANUAL);
	m_dataMinRange_NEONATAL.m_iParaDataPManual_HFO=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PMANUAL"), MINRANGE_NEO_PMANUAL);
	m_dataMinRange_NEONATAL.m_iParaDataPManual_NMODE=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_NEO_PMANUAL_NMODE"), MINRANGE_NEO_PMANUAL_NMODE);
	m_dataMinRange_NEONATAL.m_iParaDataO2=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_O2"), MINRANGE_NEO_O2);
	m_dataMinRange_NEONATAL.m_iParaDataO2Flush=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_O2FLUSH"), MINRANGE_NEO_O2FLUSH);
	m_dataMinRange_NEONATAL.m_iParaDataVLimit_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VLIMIT_TRIGGER"), MINRANGE_NEO_VLIMIT_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataVLimit_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VLIMIT_IPPV"), MINRANGE_NEO_VLIMIT_IPPV);
	m_dataMinRange_NEONATAL.m_iParaDataVGarant_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VGARANT_TRIGGER"), MINRANGE_NEO_VGARANT_TRIGGER);
	m_dataMinRange_NEONATAL.m_iParaDataVGarant_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_VGARANT_IPPV"), MINRANGE_NEO_VGARANT_IPPV);

	/*regLimit.WriteDWORD(_T("MINRANGE_NEO_TRIGGER_CONV"), MINRANGE_NEO_TRIGGER_CONV);
	regLimit.WriteDWORD(_T("MINRANGE_NEO_TRIGGER_CPAP"), MINRANGE_NEO_TRIGGER_CPAP);
	regLimit.WriteDWORD(_T("MINRANGE_NEO_TRIGGER_DUOPAP"), MINRANGE_NEO_TRIGGER_DUOPAP);
	regLimit.WriteDWORD(_T("MINRANGE_NEO_TRIGGER_NCPAP"), MINRANGE_NEO_TRIGGER_NCPAP);*/
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_CONV=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_TRIGGER_CONV"), MINRANGE_NEO_TRIGGER_CONV);
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_CPAP=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_TRIGGER_CPAP"), MINRANGE_NEO_TRIGGER_CPAP);
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_DUOPAP=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_TRIGGER_DUOPAP"), MINRANGE_NEO_TRIGGER_DUOPAP);
	m_dataMinRange_NEONATAL.m_iParaDataTrigger_NCPAP=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_TRIGGER_NCPAP"), MINRANGE_NEO_TRIGGER_NCPAP);

	
	m_dataMinRange_NEONATAL.m_iParaDataBackup=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_BACKUP"), MINRANGE_NEO_BACKUP);
	m_dataMinRange_NEONATAL.m_iParaDataHFAMPL=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFAMPL"), MINRANGE_NEO_HFAMPL);
	m_dataMinRange_NEONATAL.m_iParaDataHFAMPLmax=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFAMPLVOLG"), MINRANGE_NEO_HFAMPLVOLG);
	m_dataMinRange_NEONATAL.m_iParaDataHFFreq=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFFREQ"), MINRANGE_NEO_HFFREQ);
	m_dataMinRange_NEONATAL.m_iParaDataHFPmean=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFPMEAN"), MINRANGE_NEO_HFPMEAN);
	m_dataMinRange_NEONATAL.m_iParaDataVGarant_HFO=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_HFVGARANT"), MINRANGE_NEO_HFVGARANT);
	m_dataMinRange_NEONATAL.m_iParaDataIERatio=(eRatioIE)regLimit.ReadDWORD(_T("MINRANGE_NEO_IERATIO"), MINRANGE_NEO_IERATIO);
	m_dataMinRange_NEONATAL.m_iParaDataITIME_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_ITIME_REC"), MINRANGE_NEO_ITIME_REC);
	m_dataMinRange_NEONATAL.m_iParaDataFREQ_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FREQ_REC"), MINRANGE_NEO_FREQ_REC);
	m_dataMinRange_NEONATAL.m_iParaDataPMEAN_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_PMEAN_REC"), MINRANGE_NEO_PMEAN_REC);

	m_dataMinRange_NEONATAL.m_iParaDataFOTconv_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTCONV_AMPLITUDE"), MINRANGE_NEO_FOTCONV_AMPLITUDE);
	m_dataMinRange_NEONATAL.m_iParaDataFOTconv_FREQ=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTCONV_FREQ"), MINRANGE_NEO_FOTCONV_FREQ);
	m_dataMinRange_NEONATAL.m_iParaDataFOTconv_STEPS=1;
	//m_dataMinRange_NEONATAL.m_iParaDataFOTconv_PEEPSTART=xxx;
	//m_dataMinRange_NEONATAL.m_iParaDataFOTconv_PEEPEND=xxx;

	m_dataMinRange_NEONATAL.m_iParaDataFOThfo_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_AMPLITUDE"), MINRANGE_NEO_FOTHFO_AMPLITUDE);
	m_dataMinRange_NEONATAL.m_iParaDataFOThfo_FREQ=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_FREQ"), MINRANGE_NEO_FOTHFO_FREQ);
	m_dataMinRange_NEONATAL.m_iParaDataFOThfo_STEPS=1;//(BYTE)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_STEPS"), MINRANGE_NEO_FOTHFO_STEPS);
	//m_dataMinRange_NEONATAL.m_iParaDataFOThfo_PMEANSTART=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_PMEANSTART"), MINRANGE_NEO_FOTHFO_PMEANSTART);
	//m_dataMinRange_NEONATAL.m_iParaDataFOThfo_PMEANEND=(WORD)regLimit.ReadDWORD(_T("MINRANGE_NEO_FOTHFO_PMEANEND"), MINRANGE_NEO_FOTHFO_PMEANEND);
	
	m_dataMaxRange_PEDIATRIC.m_iParaDataITime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_TRIGGER"), MAXRANGE_PED_ITIME_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataITime_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_IPPV"), MAXRANGE_PED_ITIME_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataITime_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_NMODE"), MAXRANGE_PED_ITIME_NMODE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_TRIGGER"), MAXRANGE_PED_ETIME_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_IPPV"), MAXRANGE_PED_ETIME_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_SIMV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_SIMV"), MAXRANGE_PED_ETIME_SIMV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataETime_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ETIME_NMODE"), MAXRANGE_PED_ETIME_NMODE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_BPM_TRIGGER"), MAXRANGE_PED_BPM_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_BPM_IPPV"), MAXRANGE_PED_BPM_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_SIMV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_BPM_SIMV"), MAXRANGE_PED_BPM_SIMV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_NMODE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_BPM_NMODE"), MAXRANGE_PED_BPM_NMODE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_IFLOW_TRIGGER"), MAXRANGE_PED_IFLOW_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_IFLOW_IPPV"), MAXRANGE_PED_IFLOW_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFFlow=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFFLOW"), MAXRANGE_PED_HFFLOW);
	m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_RISETIME_TRIGGER"), MAXRANGE_PED_RISETIME_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_RISETIME_IPPV"), MAXRANGE_PED_RISETIME_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_EFLOW_TRIGGER"), MAXRANGE_PED_EFLOW_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_EFLOW_IPPV"), MAXRANGE_PED_EFLOW_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataTherapieFlow=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_THERAPYFLOW"), MAXRANGE_PED_THERAPYFLOW);
	m_dataMaxRange_PEDIATRIC.m_iParaDataFlowMin=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FLOWMIN"), MAXRANGE_PED_FLOWMIN);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PEEP_TRIGGER"), MAXRANGE_PED_PEEP_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_IPPV=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PEEP_IPPV"), MAXRANGE_PED_PEEP_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataCPAP=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_CPAP"), MAXRANGE_PED_CPAP);
	m_dataMaxRange_PEDIATRIC.m_iParaDataCPAP_NMODE=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_CPAP_NMODE"), MAXRANGE_PED_CPAP_NMODE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PINSP_TRIGGER"), MAXRANGE_PED_PINSP_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_IPPV=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PINSP_IPPV"), MAXRANGE_PED_PINSP_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMAXVOLG_TRIGGER"), MAXRANGE_PED_PMAXVOLG_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_IPPV=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMAXVOLG_IPPV"), MAXRANGE_PED_PMAXVOLG_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPpsv=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PPSV"), MAXRANGE_PED_PPSV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_CPAP=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMANUAL"), MAXRANGE_PED_PMANUAL);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_HFO=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMANUAL"), MAXRANGE_PED_PMANUAL);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_NMODE=(SHORT)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMANUAL_NMODE"), MAXRANGE_PED_PMANUAL_NMODE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataO2=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_O2"), MAXRANGE_PED_O2);
	m_dataMaxRange_PEDIATRIC.m_iParaDataO2Flush=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_O2FLUSH"), MAXRANGE_PED_O2FLUSH);
	m_dataMaxRange_PEDIATRIC.m_iParaDataVLimit_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VLIMIT_TRIGGER"), MAXRANGE_PED_VLIMIT_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataVLimit_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VLIMIT_IPPV"), MAXRANGE_PED_VLIMIT_IPPV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VGARANT_TRIGGER"), MAXRANGE_PED_VGARANT_TRIGGER);
	m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_IPPV=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_VGARANT_IPPV"), MAXRANGE_PED_VGARANT_IPPV);
	
	/*regLimit.WriteDWORD(_T("MAXRANGE_PED_TRIGGER_CONV"), MAXRANGE_PED_TRIGGER_CONV);
	regLimit.WriteDWORD(_T("MAXRANGE_PED_TRIGGER_CPAP"), MAXRANGE_PED_TRIGGER_CPAP);
	regLimit.WriteDWORD(_T("MAXRANGE_PED_TRIGGER_DUOPAP"), MAXRANGE_TRIGGER_OFF);
	regLimit.WriteDWORD(_T("MAXRANGE_PED_TRIGGER_NCPAP"), MAXRANGE_TRIGGER_OFF);*/
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_CONV=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_CONV"), MAXRANGE_PED_TRIGGER_CONV);
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_CPAP=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_CPAP"), MAXRANGE_PED_TRIGGER_CPAP);
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_DUOPAP=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_DUOPAP_OFF"), MAXRANGE_TRIGGER_OFF);
	m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_NCPAP=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_TRIGGER_NCPAP_OFF"), MAXRANGE_TRIGGER_OFF);
	
	m_dataMaxRange_PEDIATRIC.m_iParaDataBackup=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_BACKUP"), MAXRANGE_PED_BACKUP);
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFFreq=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFFREQ"), MAXRANGE_PED_HFFREQ);
	if(getModel()->getDATAHANDLER()->IsAccuSupply()==true)
	{
		m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPL=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_ACCU_HFAMPL"), MAXRANGE_ACCU_HFAMPL);
		m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPLmax=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_ACCU_HFAMPLVOLG"), MAXRANGE_ACCU_HFAMPLVOLG);
	}
	else
	{
		m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPL=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFAMPL"), MAXRANGE_PED_HFAMPL);
		m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPLmax=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFAMPLVOLG"), MAXRANGE_PED_HFAMPLVOLG);
	}
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFPmean=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFPMEAN"), MAXRANGE_PED_HFPMEAN);
	m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_HFO=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_HFVGARANT"), MAXRANGE_PED_HFVGARANT);
	m_dataMaxRange_PEDIATRIC.m_iParaDataIERatio=(eRatioIE)regLimit.ReadDWORD(_T("MAXRANGE_PED_IERATIO"), MAXRANGE_PED_IERATIO);
	m_dataMaxRange_PEDIATRIC.m_iParaDataITIME_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_ITIME_REC"), MAXRANGE_PED_ITIME_REC);
	m_dataMaxRange_PEDIATRIC.m_iParaDataFREQ_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FREQ_REC"), MAXRANGE_PED_FREQ_REC);
	m_dataMaxRange_PEDIATRIC.m_iParaDataPMEAN_REC=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_PMEAN_REC"), MAXRANGE_PED_PMEAN_REC);

	m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTCONV_AMPLITUDE"), MAXRANGE_PED_FOTCONV_AMPLITUDE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_FREQ=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTCONV_FREQ"), MAXRANGE_PED_FOTCONV_FREQ);
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_STEPS=MAXSIZE_FOT_STEPS;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_PEEPSTART=xxx;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_PEEPEND=xxx;
	
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_AMPLITUDE"), MAXRANGE_PED_FOTHFO_AMPLITUDE);
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_FREQ=(BYTE)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_FREQ"), MAXRANGE_PED_FOTHFO_FREQ);
	m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_STEPS=MAXSIZE_FOT_STEPS;
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_PMEANSTART"), MAXRANGE_PED_FOTHFO_PMEANSTART);
	//m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_PMEANEND=(WORD)regLimit.ReadDWORD(_T("MAXRANGE_PED_FOTHFO_PMEANEND"), MAXRANGE_PED_FOTHFO_PMEANEND);
	

	m_dataMinRange_PEDIATRIC.m_iParaDataITime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ITIME_TRIGGER"), MINRANGE_PED_ITIME_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataITime_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ITIME_IPPV"), MINRANGE_PED_ITIME_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataITime_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ITIME_NMODE"), MINRANGE_PED_ITIME_NMODE);
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ETIME_TRIGGER"), MINRANGE_PED_ETIME_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ETIME_IPPV"), MINRANGE_PED_ETIME_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_SIMV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ETIME_SIMV"), MINRANGE_PED_ETIME_SIMV);
	m_dataMinRange_PEDIATRIC.m_iParaDataETime_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ETIME_NMODE"), MINRANGE_PED_ETIME_NMODE);
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_BPM_TRIGGER"), MINRANGE_PED_BPM_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_BPM_IPPV"), MINRANGE_PED_BPM_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_SIMV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_BPM_SIMV"), MINRANGE_PED_BPM_SIMV);
	m_dataMinRange_PEDIATRIC.m_iParaDataBPM_NMODE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_BPM_NMODE"), MINRANGE_PED_BPM_NMODE);
	m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_IFLOW_TRIGGER"), MINRANGE_PED_IFLOW_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_IFLOW_IPPV"), MINRANGE_PED_IFLOW_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataHFFlow=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_HFFLOW"), MINRANGE_PED_HFFLOW);
	m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_RISETIME_TRIGGER"), MINRANGE_PED_RISETIME_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_RISETIME_IPPV"), MINRANGE_PED_RISETIME_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_EFLOW_TRIGGER"), MINRANGE_PED_EFLOW_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_EFLOW_IPPV"), MINRANGE_PED_EFLOW_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataTherapieFlow=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_THERAPYFLOW"), MINRANGE_PED_THERAPYFLOW);
	m_dataMinRange_PEDIATRIC.m_iParaDataFlowMin=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FLOWMIN"), MINRANGE_PED_FLOWMIN);
	m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PEEP_TRIGGER"), MINRANGE_PED_PEEP_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_IPPV=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PEEP_IPPV"), MINRANGE_PED_PEEP_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_RISETIME=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PEEP_RISETIME"), MINRANGE_PED_PEEP_RISETIME);
	m_dataMinRange_PEDIATRIC.m_iParaDataCPAP=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_CPAP"), MINRANGE_PED_CPAP);
	m_dataMinRange_PEDIATRIC.m_iParaDataCPAP_NMODE=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_CPAP_NMODE"), MINRANGE_PED_CPAP_NMODE);
	m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PINSP_TRIGGER"), MINRANGE_PED_PINSP_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_IPPV=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PINSP_IPPV"), MINRANGE_PED_PINSP_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PMAXVOLG_TRIGGER"), MINRANGE_PED_PMAXVOLG_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_IPPV=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PMAXVOLG_IPPV"), MINRANGE_PED_PMAXVOLG_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataPpsv=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PPSV"), MINRANGE_PED_PPSV);
	m_dataMinRange_PEDIATRIC.m_iParaDataPManual_CPAP=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PMANUAL"), MINRANGE_PED_PMANUAL);
	m_dataMinRange_PEDIATRIC.m_iParaDataPManual_HFO=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PMANUAL"), MINRANGE_PED_PMANUAL);
	m_dataMinRange_PEDIATRIC.m_iParaDataPManual_NMODE=(SHORT)regLimit.ReadDWORD(_T("MINRANGE_PED_PMANUAL_NMODE"), MINRANGE_PED_PMANUAL_NMODE);
	m_dataMinRange_PEDIATRIC.m_iParaDataO2=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_O2"), MINRANGE_PED_O2);
	m_dataMinRange_PEDIATRIC.m_iParaDataO2Flush=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_O2FLUSH"), MINRANGE_PED_O2FLUSH);
	m_dataMinRange_PEDIATRIC.m_iParaDataVLimit_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_VLIMIT_TRIGGER"), MINRANGE_PED_VLIMIT_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataVLimit_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_VLIMIT_IPPV"), MINRANGE_PED_VLIMIT_IPPV);
	m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_TRIGGER=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_VGARANT_TRIGGER"), MINRANGE_PED_VGARANT_TRIGGER);
	m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_IPPV=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_VGARANT_IPPV"), MINRANGE_PED_VGARANT_IPPV);
	
	/*regLimit.WriteDWORD(_T("MINRANGE_PED_TRIGGER_CONV"), MINRANGE_PED_TRIGGER_CONV);
	regLimit.WriteDWORD(_T("MINRANGE_PED_TRIGGER_CPAP"), MINRANGE_PED_TRIGGER_CPAP);
	regLimit.WriteDWORD(_T("MINRANGE_PED_TRIGGER_DUOPAP"), MINRANGE_PED_TRIGGER_DUOPAP);
	regLimit.WriteDWORD(_T("MINRANGE_PED_TRIGGER_NCPAP"), MINRANGE_PED_TRIGGER_NCPAP);*/
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_CONV=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_CONV"), MINRANGE_PED_TRIGGER_CONV);
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_CPAP=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_CPAP"), MINRANGE_PED_TRIGGER_CPAP);
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_DUOPAP=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_DUOPAP"), MINRANGE_PED_TRIGGER_DUOPAP);
	m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_NCPAP=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_TRIGGER_NCPAP"), MINRANGE_PED_TRIGGER_NCPAP);
	
	m_dataMinRange_PEDIATRIC.m_iParaDataBackup=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_BACKUP"), MINRANGE_PED_BACKUP);
	m_dataMinRange_PEDIATRIC.m_iParaDataHFAMPL=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_HFAMPL"), MINRANGE_PED_HFAMPL);
	m_dataMinRange_PEDIATRIC.m_iParaDataHFAMPLmax=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_HFAMPLVOLG"), MINRANGE_PED_HFAMPLVOLG);
	m_dataMinRange_PEDIATRIC.m_iParaDataHFFreq=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_HFFREQ"), MINRANGE_PED_HFFREQ);
	m_dataMinRange_PEDIATRIC.m_iParaDataHFPmean=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_HFPMEAN"), MINRANGE_PED_HFPMEAN);
	m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_HFO=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_HFVGARANT"), MINRANGE_PED_HFVGARANT);
	m_dataMinRange_PEDIATRIC.m_iParaDataIERatio=(eRatioIE)regLimit.ReadDWORD(_T("MINRANGE_PED_IERATIO"), MINRANGE_PED_IERATIO);
	m_dataMinRange_PEDIATRIC.m_iParaDataITIME_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_ITIME_REC"), MINRANGE_PED_ITIME_REC);
	m_dataMinRange_PEDIATRIC.m_iParaDataFREQ_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FREQ_REC"), MINRANGE_PED_FREQ_REC);
	m_dataMinRange_PEDIATRIC.m_iParaDataPMEAN_REC=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_PMEAN_REC"), MINRANGE_PED_PMEAN_REC);

	m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTCONV_AMPLITUDE"), MINRANGE_PED_FOTCONV_AMPLITUDE);
	m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_FREQ=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTCONV_FREQ"), MINRANGE_PED_FOTCONV_FREQ);
	m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_STEPS=1;
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_PEEPSTART=xxx;
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_PEEPEND=xxx;

	m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTHFO_AMPLITUDE"), MINRANGE_PED_FOTHFO_AMPLITUDE);
	m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_FREQ=(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTHFO_FREQ"), MINRANGE_PED_FOTHFO_FREQ);
	m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_STEPS=1;//(BYTE)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTHFO_STEPS"), MINRANGE_PED_FOTHFO_STEPS);
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTHFO_PMEANSTART"), MINRANGE_PED_FOTHFO_PMEANSTART);
	//m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_PMEANEND=(WORD)regLimit.ReadDWORD(_T("MINRANGE_PED_FOTHFO_PMEANEND"), MINRANGE_PED_FOTHFO_PMEANEND);
	
}

/**********************************************************************************************//**
 * Check i erelation
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CParaData::checkIErelation()
{
	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		//IPPV
		if(false==getModel()->getDATAHANDLER()->ConfirmItoBPMrelation(m_data.m_iParaDataITime_IPPV,m_data.m_iParaDataBPM_IPPV,m_data.m_iParaDataETime_IPPV))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0257 m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime_IPPV,m_data.m_iParaDataBPM_IPPV,m_data.m_iParaDataETime_IPPV);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeMinPara_IPPV();
			int iUpperLimitITime=GetITimeMaxPara_IPPV();

			int iLowerLimitBPM=GetBPMMinPara_IPPV();
			int iUpperLimitBPM=GetBPMMaxPara_IPPV();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime_IPPV>=iLowerLimitITime
				&&	m_data.m_iParaDataITime_IPPV<=iUpperLimitITime
				&&	m_data.m_iParaDataBPM_IPPV>=iLowerLimitBPM
				&&	m_data.m_iParaDataBPM_IPPV<=iUpperLimitBPM)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateETime_ITimeChanged_IPPV(m_data.m_iParaDataITime_IPPV,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_IPPV=FACTORY_IPPV_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_IPPV_16,m_data.m_iParaDataBPM_IPPV);
				m_data.m_iParaDataITime_IPPV=FACTORY_IPPV_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_IPPV_16,m_data.m_iParaDataITime_IPPV);
				m_data.m_iParaDataETime_IPPV=FACTORY_IPPV_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_IPPV_16,m_data.m_iParaDataETime_IPPV);
			}
		}

		//normal mode (TRIGGER)
		if(false==getModel()->getDATAHANDLER()->ConfirmItoBPMrelation(m_data.m_iParaDataITime_TRIGGER,m_data.m_iParaDataBPM_TRIGGER,m_data.m_iParaDataETime_TRIGGER))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0258 m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime_TRIGGER,m_data.m_iParaDataBPM_TRIGGER,m_data.m_iParaDataETime_TRIGGER);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeMinPara_TRIGGER();
			int iUpperLimitITime=GetITimeMaxPara_TRIGGER();

			int iLowerLimitBPM=GetBPMMinPara_TRIGGER();
			int iUpperLimitBPM=GetBPMMaxPara_TRIGGER();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime_TRIGGER>=iLowerLimitITime
				&&	m_data.m_iParaDataITime_TRIGGER<=iUpperLimitITime
				&&	m_data.m_iParaDataBPM_TRIGGER>=iLowerLimitBPM
				&&	m_data.m_iParaDataBPM_TRIGGER<=iUpperLimitBPM)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateETime_ITimeChanged_TRIGGER(m_data.m_iParaDataITime_TRIGGER,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_TRIGGER=FACTORY_IPPV_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_TRIGGER_16,m_data.m_iParaDataBPM_TRIGGER);
				m_data.m_iParaDataITime_TRIGGER=FACTORY_IPPV_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_TRIGGER_16,m_data.m_iParaDataITime_TRIGGER);
				m_data.m_iParaDataETime_TRIGGER=FACTORY_IPPV_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_TRIGGER_16,m_data.m_iParaDataETime_TRIGGER);
			}
		}

		//SIMV mode 
		/*if(false==getModel()->getDATAHANDLER()->ConfirmItoBPMrelation(m_data.m_iParaDataITime,m_data.m_iParaDataBPM_SIMV,m_data.m_iParaDataETime_SIMV))
		{
			CStringW sz=_T("");
			sz.Format(_T("ERROR CParaDataPSV::Init2() RM_SET_ItoBPM SIMV m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime,m_data.m_iParaDataBPM_SIMV,m_data.m_iParaDataETime_SIMV);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeMinPara();
			int iUpperLimitITime=GetITimeMaxPara();

			int iLowerLimitBPM=GetBPMMinPara();
			int iUpperLimitBPM=GetBPMMaxPara();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime>=iLowerLimitITime
				&&	m_data.m_iParaDataITime<=iUpperLimitITime
				&&	m_data.m_iParaDataBPM_SIMV>=iLowerLimitBPM
				&&	m_data.m_iParaDataBPM_SIMV<=iUpperLimitBPM)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateETime_ITimeChanged(m_data.m_iParaDataITime,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_SIMV=FACTORY_IPPV_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_16,m_data.m_iParaDataBPM_SIMV);
				m_data.m_iParaDataITime=FACTORY_IPPV_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_16,m_data.m_iParaDataITime);
				m_data.m_iParaDataETime_SIMV=FACTORY_IPPV_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_16,m_data.m_iParaDataETime_SIMV);
			}
		}*/

		//NMODE
		if(false==getModel()->getDATAHANDLER()->ConfirmItoBPMrelation(m_data.m_iParaDataITime_NMODE,m_data.m_iParaDataBPM_NMODE,m_data.m_iParaDataETime_NMODE))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0259 m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime_NMODE,m_data.m_iParaDataBPM_NMODE,m_data.m_iParaDataETime_NMODE);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeNMODEMinPara();
			int iUpperLimitITime=GetITimeNMODEMaxPara();

			int iLowerLimitBPM=GetBPMNMODEMinPara();
			int iUpperLimitBPM=GetBPMNMODEMaxPara();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime_NMODE>=iLowerLimitITime
				&&	m_data.m_iParaDataITime_NMODE<=iUpperLimitITime
				&&	m_data.m_iParaDataBPM_NMODE>=iLowerLimitBPM
				&&	m_data.m_iParaDataBPM_NMODE<=iUpperLimitBPM)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateETime_ITimeChanged_NMODE(m_data.m_iParaDataITime_NMODE,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_NMODE=FACTORY_NMODE_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_NMODE_16,m_data.m_iParaDataBPM_NMODE);
				m_data.m_iParaDataITime_NMODE=FACTORY_NMODE_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_NMODE_16,m_data.m_iParaDataITime_NMODE);
				m_data.m_iParaDataETime_NMODE=FACTORY_NMODE_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_NMODE_16,m_data.m_iParaDataETime_NMODE);
			}
		}
	}
	else
	{
		//IPPV
		if(false==getModel()->getDATAHANDLER()->ConfirmItoErelation(m_data.m_iParaDataITime_IPPV,m_data.m_iParaDataETime_IPPV,m_data.m_iParaDataBPM_IPPV))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0294 m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime_IPPV,m_data.m_iParaDataBPM_IPPV,m_data.m_iParaDataETime_IPPV);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeMinPara_IPPV();
			int iUpperLimitITime=GetITimeMaxPara_IPPV();

			int iLowerLimitETime=GetETIMEMinPara_IPPV();
			int iUpperLimitETime=GetETIMEMaxPara_IPPV();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime_IPPV>=iLowerLimitITime
				&&	m_data.m_iParaDataITime_IPPV<=iUpperLimitITime
				&&	m_data.m_iParaDataETime_IPPV>=iLowerLimitETime
				&&	m_data.m_iParaDataETime_IPPV<=iUpperLimitETime)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateBPM_ITimeChanged_IPPV(m_data.m_iParaDataITime_IPPV,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_IPPV=FACTORY_IPPV_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_IPPV_16,m_data.m_iParaDataBPM_IPPV);
				m_data.m_iParaDataITime_IPPV=FACTORY_IPPV_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_IPPV_16,m_data.m_iParaDataITime_IPPV);
				m_data.m_iParaDataETime_IPPV=FACTORY_IPPV_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_IPPV_16,m_data.m_iParaDataETime_IPPV);
			}

		}

		//normal mode (TRIGGER)
		if(false==getModel()->getDATAHANDLER()->ConfirmItoErelation(m_data.m_iParaDataITime_TRIGGER,m_data.m_iParaDataETime_TRIGGER,m_data.m_iParaDataBPM_TRIGGER))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0295 m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime_TRIGGER,m_data.m_iParaDataBPM_TRIGGER,m_data.m_iParaDataETime_TRIGGER);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeMinPara_TRIGGER();
			int iUpperLimitITime=GetITimeMaxPara_TRIGGER();

			int iLowerLimitETime=GetETIMEMinPara_TRIGGER();
			int iUpperLimitETime=GetETIMEMaxPara_TRIGGER();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime_TRIGGER>=iLowerLimitITime
				&&	m_data.m_iParaDataITime_TRIGGER<=iUpperLimitITime
				&&	m_data.m_iParaDataETime_TRIGGER>=iLowerLimitETime
				&&	m_data.m_iParaDataETime_TRIGGER<=iUpperLimitETime)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateBPM_ITimeChanged_TRIGGER(m_data.m_iParaDataITime_TRIGGER,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_TRIGGER=FACTORY_IPPV_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_TRIGGER_16,m_data.m_iParaDataBPM_TRIGGER);
				m_data.m_iParaDataITime_TRIGGER=FACTORY_IPPV_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_TRIGGER_16,m_data.m_iParaDataITime_TRIGGER);
				m_data.m_iParaDataETime_TRIGGER=FACTORY_IPPV_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_TRIGGER_16,m_data.m_iParaDataETime_TRIGGER);
			}

		}

		//SIMV mode
		/*if(false==getModel()->getDATAHANDLER()->ConfirmItoErelation(m_data.m_iParaDataITime,m_data.m_iParaDataETime_SIMV,m_data.m_iParaDataBPM_SIMV))
		{
			CStringW sz=_T("");
			sz.Format(_T("ERROR CParaDataPSV::Init5() RM_SET_ItoE m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime,m_data.m_iParaDataBPM_SIMV,m_data.m_iParaDataETime_SIMV);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeMinPara();
			int iUpperLimitITime=GetITimeMaxPara();

			int iLowerLimitETime=GetETIMEMinPara();
			int iUpperLimitETime=GetETIMEMaxPara();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime>=iLowerLimitITime
				&&	m_data.m_iParaDataITime<=iUpperLimitITime
				&&	m_data.m_iParaDataETime_SIMV>=iLowerLimitETime
				&&	m_data.m_iParaDataETime_SIMV<=iUpperLimitETime)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateBPM_ITimeChanged(m_data.m_iParaDataITime,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_SIMV=FACTORY_IPPV_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_16,m_data.m_iParaDataBPM_SIMV);
				m_data.m_iParaDataITime=FACTORY_IPPV_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_16,m_data.m_iParaDataITime);
				m_data.m_iParaDataETime_SIMV=FACTORY_IPPV_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_16,m_data.m_iParaDataETime_SIMV);
			}

		}*/

		//NMODE
		if(false==getModel()->getDATAHANDLER()->ConfirmItoErelation(m_data.m_iParaDataITime_NMODE,m_data.m_iParaDataETime_NMODE,m_data.m_iParaDataBPM_NMODE))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0296 m_iITime%d m_iBPM%d m_iETime%d."),m_data.m_iParaDataITime_NMODE,m_data.m_iParaDataBPM_NMODE,m_data.m_iParaDataETime_NMODE);
			theApp.WriteLog(sz);

			int iLowerLimitITime=GetITimeNMODEMinPara();
			int iUpperLimitITime=GetITimeNMODEMaxPara();

			int iLowerLimitETime=GetETIMENMODEMinPara();
			int iUpperLimitETime=GetETIMENMODEMaxPara();

			bool bCalcOk=true;

			if(		m_data.m_iParaDataITime_NMODE>=iLowerLimitITime
				&&	m_data.m_iParaDataITime_NMODE<=iUpperLimitITime
				&&	m_data.m_iParaDataETime_NMODE>=iLowerLimitETime
				&&	m_data.m_iParaDataETime_NMODE<=iUpperLimitETime)
			{
				if(false==getModel()->getDATAHANDLER()->InitialCalculateBPM_ITimeChanged_NMODE(m_data.m_iParaDataITime_NMODE,false))
				{
					bCalcOk=false;
				}
			}
			else
			{
				bCalcOk=false;
			}

			if(!bCalcOk)
			{
				m_data.m_iParaDataBPM_NMODE=FACTORY_NMODE_BPM;
				getModel()->getI2C()->WriteConfigWord(PARA_BPM_NMODE_16,m_data.m_iParaDataBPM_NMODE);
				m_data.m_iParaDataITime_NMODE=FACTORY_NMODE_ITIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ITIME_NMODE_16,m_data.m_iParaDataITime_NMODE);
				m_data.m_iParaDataETime_NMODE=FACTORY_NMODE_ETIME;
				getModel()->getI2C()->WriteConfigWord(PARA_ETIME_NMODE_16,m_data.m_iParaDataETime_NMODE);
			}
		}
	}
}

/**********************************************************************************************//**
 * Gets para data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pData	If non-null, the data.
 **************************************************************************************************/

void CParaData::GetParaData(PARA_DATA* pData)
{
	memcpy(pData,&m_data,sizeof(PARA_DATA));
}

/**********************************************************************************************//**
 * Sets data from mode
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	mode	The mode.
 **************************************************************************************************/

void CParaData::SetDataFromMode(eVentMode mode)
{
	switch(mode)
	{
	case VM_IPPV:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_PSV:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_SIMV:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_SIMVPSV:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_SIPPV:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_CPAP:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_DUOPAP:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_NCPAP:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_THERAPIE:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_HFO:
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_NEONATAL); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_NEONATAL);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->GetMinParaData(&m_dataMinRange_PEDIATRIC); 
				getModel()->getDATAHANDLER()->PARADATA()->GetMaxParaData(&m_dataMaxRange_PEDIATRIC);
			}
		}
		break;
	case VM_PRE_IPPV:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);

			getModel()->getDATAHANDLER()->PARADATA()->SetPEEPPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_IPPV(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetRisetimePara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetRisetimePara_IPPV(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetIFlowPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_IPPV(),false,true);
			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW()
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==getModel()->getDATAHANDLER()->GetExhalValvCalMode())
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetEFLOWPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_IPPV(),false,true);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetEFLOWPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetEFLOWPara_IPPV(),false,true);
			}
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			if(getModel()->getDATAHANDLER()->PRESET()->IsVGarantStateOn_IPPV())
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetPmaxVolGPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_IPPV(),false,true);
				getModel()->getDATAHANDLER()->PARADATA()->SetPINSPPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_IPPV(),false,false);
				getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_IPPV(),false,true,true);

			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetPmaxVolGPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_IPPV(),false,false);
				getModel()->getDATAHANDLER()->PARADATA()->SetPINSPPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_IPPV(),false,true);

				getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_IPPV(),getModel()->getDATAHANDLER()->PRESET()->IsVLimitParamOn_IPPV(),false,true);
			}

			getModel()->getDATAHANDLER()->PARADATA()->SetVGarantPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetVGarantPara_IPPV(),false,true);

			getModel()->getDATAHANDLER()->PARADATA()->SetBPMPara_IPPV(getModel()->getDATAHANDLER()->PRESET()->GetBPMPara_IPPV(),false,true);
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);

			getModel()->getDATAHANDLER()->PARADATA()->SetPEEPPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetRisetimePara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetRisetimePara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetIFlowPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_TRIGGER(),false,true);
			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW()
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==getModel()->getDATAHANDLER()->GetExhalValvCalMode())
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetEFLOWPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_TRIGGER(),false,true);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetEFLOWPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetEFLOWPara_TRIGGER(),false,true);
			}
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_CONV(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_CONV(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			if(getModel()->getDATAHANDLER()->PRESET()->IsVGarantStateOn_TRIGGER())
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetPmaxVolGPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_TRIGGER(),false,true);
				getModel()->getDATAHANDLER()->PARADATA()->SetPINSPPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER(),false,false);
				getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_TRIGGER(),false,true,true);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetPmaxVolGPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_TRIGGER(),false,false);
				getModel()->getDATAHANDLER()->PARADATA()->SetPINSPPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER(),false,true);
				getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_TRIGGER(),getModel()->getDATAHANDLER()->PRESET()->IsVLimitParamOn_TRIGGER(),false,true);
			}
			getModel()->getDATAHANDLER()->PARADATA()->SetVGarantPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetVGarantPara_TRIGGER(),false,true);

			getModel()->getDATAHANDLER()->PARADATA()->SetBPMPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetBPMPara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_CONV(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_CONV(),false,true);
		}
		break;
	case VM_PRE_HFO:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);
			//getModel()->getDATAHANDLER()->PARADATA()->SetPINSPPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPinspPara_TRIGGER(),true,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFVGarantPara(getModel()->getDATAHANDLER()->PRESET()->GetHFVGarantPara(),false,true);

			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			getModel()->getDATAHANDLER()->PARADATA()->SetFreqRecPara(getModel()->getDATAHANDLER()->PRESET()->GetFreqRecPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFPMeanRecPara(getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanRecPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetITimeRecPara(getModel()->getDATAHANDLER()->PRESET()->GetITimeRecPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFFlowPara(getModel()->getDATAHANDLER()->PRESET()->GetHFFlowPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFAMPLPara(getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFPMeanPara(getModel()->getDATAHANDLER()->PRESET()->GetHFPMeanPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFAMPLmaxPara(getModel()->getDATAHANDLER()->PRESET()->GetHFAMPLmaxPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetIERatioParaHFO(getModel()->getDATAHANDLER()->PRESET()->GetIERatioParaHFO(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetHFFreqPara(getModel()->getDATAHANDLER()->PRESET()->GetHFFreqPara(),false,true);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMVPSV:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);

			getModel()->getDATAHANDLER()->PARADATA()->SetPEEPPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetRisetimePara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetRisetimePara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetIFlowPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_TRIGGER(),false,true);
			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW()
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==getModel()->getDATAHANDLER()->GetExhalValvCalMode())
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetEFLOWPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetIFlowPara_TRIGGER(),false,true);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetEFLOWPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetEFLOWPara_TRIGGER(),false,true);
			}
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_CONV(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_CONV(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			//kein PinspVolG an SERIAL schicken!
			getModel()->getDATAHANDLER()->PARADATA()->SetPmaxVolGPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPmaxVolGPara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetPINSPPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetPINSPPara_TRIGGER(),false,true);

			if(getModel()->getDATAHANDLER()->PRESET()->IsVGarantStateOn_TRIGGER())
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_TRIGGER(),false,false,true);
			}
			else
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetVLimitParam_TRIGGER(),getModel()->getDATAHANDLER()->PRESET()->IsVLimitParamOn_TRIGGER(),false,true);
			}

			getModel()->getDATAHANDLER()->PARADATA()->SetVGarantPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetVGarantPara_TRIGGER(),false,true);

			getModel()->getDATAHANDLER()->PARADATA()->SetPpsvPara(getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetITimePara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetITimePara_TRIGGER(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetETIMEPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetETIMEPara_TRIGGER(), false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetBPMPara_TRIGGER(getModel()->getDATAHANDLER()->PRESET()->GetBPMPara_TRIGGER(),false,true);
		}
		break;
	case VM_PRE_CPAP:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_CPAP(TRIGGER_SENSITIVITY_AUTO,false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			getModel()->getDATAHANDLER()->PARADATA()->SetFlowminPara(getModel()->getDATAHANDLER()->PRESET()->GetFlowminPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetCPAPPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetPManualCPAPPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualCPAPPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetBackupPara(getModel()->getDATAHANDLER()->PRESET()->GetBackupPara(),false,true);
		}
		break;
	case VM_PRE_NCPAP:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_NCPAP(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_NCPAP(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			getModel()->getDATAHANDLER()->PARADATA()->SetCPAPNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetPManualNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara(),false,true);
		}
		break;
	case VM_PRE_THERAPIE:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			getModel()->getDATAHANDLER()->PARADATA()->SetTherapieFLOWPara(getModel()->getDATAHANDLER()->PRESET()->GetTherapieFLOWPara(),false,true);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			getModel()->getDATAHANDLER()->PRESET()->GetParaData(&m_data);
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_DUOPAP(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_DUOPAP(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2Para(getModel()->getDATAHANDLER()->PRESET()->GetO2Para(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetO2FlushPara(getModel()->getDATAHANDLER()->PRESET()->GetO2FlushPara(),true);

			getModel()->getDATAHANDLER()->PARADATA()->SetCPAPNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetCPAPNMODEPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetPManualNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetPManualNMODEPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetTriggerPara_DUOPAP(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_DUOPAP(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetITimeNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetETIMENMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetETIMENMODEPara(),false,true);
			getModel()->getDATAHANDLER()->PARADATA()->SetBPMNMODEPara(getModel()->getDATAHANDLER()->PRESET()->GetBPMNMODEPara(),false,true);
		}
		break;
	default:
		break;
	}

}

/**********************************************************************************************//**
 * Gets maximum para data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pData	If non-null, the data.
 **************************************************************************************************/

void CParaData::GetMaxParaData(PARA_DATA* pData)
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		memcpy(pData,&m_dataMaxRange_NEONATAL,sizeof(PARA_DATA));
	else
		memcpy(pData,&m_dataMaxRange_PEDIATRIC,sizeof(PARA_DATA));
}

/**********************************************************************************************//**
 * Sets maximum para data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pData	If non-null, the data.
 **************************************************************************************************/

void CParaData::SetMaxParaData(PARA_DATA* pData)
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		memcpy(&m_dataMaxRange_NEONATAL,pData,sizeof(PARA_DATA));
	else
		memcpy(&m_dataMaxRange_PEDIATRIC,pData,sizeof(PARA_DATA));
}

/**********************************************************************************************//**
 * Gets minimum para data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pData	If non-null, the data.
 **************************************************************************************************/

void CParaData::GetMinParaData(PARA_DATA* pData)
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		memcpy(pData,&m_dataMinRange_NEONATAL,sizeof(PARA_DATA));
	else
		memcpy(pData,&m_dataMinRange_PEDIATRIC,sizeof(PARA_DATA));
}

/**********************************************************************************************//**
 * Sets minimum para data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pData	If non-null, the data.
 **************************************************************************************************/

void CParaData::SetMinParaData(PARA_DATA* pData)
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		memcpy(pData,&m_dataMinRange_NEONATAL,sizeof(PARA_DATA));
	else
		memcpy(&m_dataMinRange_PEDIATRIC,pData,sizeof(PARA_DATA));
}

/**********************************************************************************************//**
 * Gets backup para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The backup para.
 **************************************************************************************************/

BYTE CParaData::GetBackupPara()
{
	return m_data.m_iParaDataBackup;
}

/**********************************************************************************************//**
 * Sets backup para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetBackupPara(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataBackup=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataBackup(value);
		if(bSend)
		{
			getModel()->Send_PARA_BACKUP(value,false,true);
		}
	}
	
}

/**********************************************************************************************//**
 * Gets cpap para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpap para.
 **************************************************************************************************/

SHORT CParaData::GetCPAPPara()
{
	return GetPEEPPara_IPPV();
	//return m_data.m_iParaDataCPAP;
}

/**********************************************************************************************//**
 * Sets cpap para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetCPAPPara(SHORT value, bool bSend, bool bConfig)
{
	//m_data.m_iParaDataCPAP=value;

	SetPEEPPara_IPPV(value,false,bConfig);

	//DEBUGMSG(TRUE, (TEXT("SetCPAPPara %d\r\n"),value));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataCPAP(value);
		if(bSend)
		{
			getModel()->Send_PARA_PEEP(value,true,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets cpapnmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpapnmode para.
 **************************************************************************************************/

SHORT CParaData::GetCPAPNMODEPara()
{
	return m_data.m_iParaDataCPAP_NMODE;
}

/**********************************************************************************************//**
 * Sets cpapnmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetCPAPNMODEPara(SHORT value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataCPAP_NMODE=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataCPAP_NMODE(value);
		if(bSend)
		{
			getModel()->Send_PARA_PEEP(value,true,true);
		}

	}
}

/**********************************************************************************************//**
 * Gets p manual hfo para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual hfo para.
 **************************************************************************************************/

SHORT CParaData::GetPManualHFOPara()
{
	return m_data.m_iParaDataPManual_HFO;
}

/**********************************************************************************************//**
 * Sets p manual hfo para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPManualHFOPara(SHORT value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataPManual_HFO=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPManual_HFO(value);
		if(bSend)
		{
			getModel()->Send_PARA_PINSP(value,true,true);//newVG
		}
	}
}

/**********************************************************************************************//**
 * Gets p manual cpap para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual cpap para.
 **************************************************************************************************/

SHORT CParaData::GetPManualCPAPPara()
{
	return GetPINSPPara_IPPV();
	//return m_data.m_iParaDataPManual_CPAP;
}

/**********************************************************************************************//**
 * Sets p manual cpap para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPManualCPAPPara(SHORT value, bool bSend, bool bConfig)
{
	SetPINSPPara_IPPV(value, false, bConfig);
	//m_data.m_iParaDataPManual_CPAP=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPManual_CPAP(value);
		if(bSend)
		{
			getModel()->Send_PARA_PINSP(value,true,true);//newVG
		}
	}
}

/**********************************************************************************************//**
 * Gets p manual nmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual nmode para.
 **************************************************************************************************/

SHORT CParaData::GetPManualNMODEPara()
{
	return m_data.m_iParaDataPManual_NMODE;
}

/**********************************************************************************************//**
 * Sets p manual nmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPManualNMODEPara(SHORT value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataPManual_NMODE=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPManual_NMODE(value);
		if(bSend)
		{
			getModel()->Send_PARA_PINSP(value,true,true);//newVG
		}
	}
}

/**********************************************************************************************//**
 * Gets pinsp para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pinsp para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPINSPPara_TRIGGER()
{
	return m_data.m_iParaDataPInsp_TRIGGER;
}

/**********************************************************************************************//**
 * Gets pinsp para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pinsp para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPINSPPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataPInsp_IPPV;
#else
	return m_data.m_iParaDataPInsp_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets pmax volume g para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pmax volume g para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPmaxVolGPara_TRIGGER()
{
	return m_data.m_iParaDataPmaxVolG_TRIGGER;
}

/**********************************************************************************************//**
 * Gets pmax volume g para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pmax volume g para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPmaxVolGPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataPmaxVolG_IPPV;
#else
	return m_data.m_iParaDataPmaxVolG_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets pmax volume g para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPmaxVolGPara_TRIGGER(SHORT val, bool bSend, bool bConfig)
{
	m_data.m_iParaDataPmaxVolG_TRIGGER=val;

	//DEBUGMSG(TRUE, (TEXT("temp PmaxVolGPara %d\r\n"),val));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPmaxVolG_TRIGGER(val);
		//DEBUGMSG(TRUE, (TEXT("config PmaxVolGPara %d\r\n"),val));

		if(bSend)
		{
			getModel()->Send_PARA_PMAXVG(val,true,true);//newVG
		}
	}
}

/**********************************************************************************************//**
 * Sets pmax volume g para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPmaxVolGPara_IPPV(SHORT val, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataPmaxVolG_IPPV=val;

	//DEBUGMSG(TRUE, (TEXT("temp PmaxVolGPara %d\r\n"),val));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPmaxVolG_IPPV(val);
		//DEBUGMSG(TRUE, (TEXT("config PmaxVolGPara %d\r\n"),val));

		if(bSend)
		{
			getModel()->Send_PARA_PMAXVG(val,true,true);//newVG
		}
	}
#else
	m_data.m_iParaDataPmaxVolG_TRIGGER=val;

	DEBUGMSG(TRUE, (TEXT("temp PmaxVolGPara %d\r\n"),val));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPmaxVolG_TRIGGER(val);
		//DEBUGMSG(TRUE, (TEXT("config PmaxVolGPara %d\r\n"),val));

		if(bSend)
		{
			getModel()->Send_PARA_PMAXVG(val,true,true);//newVG
		}
	}
#endif
	
}

/**********************************************************************************************//**
 * Sets pinsp para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPINSPPara_TRIGGER(SHORT val, bool bSend, bool bConfig)
{
	//m_data.m_iParaDataPInsp=(val/5)*5;
	m_data.m_iParaDataPInsp_TRIGGER=val;

	DEBUGMSG(TRUE, (TEXT("temp PinspPara %d\r\n"),val));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPInsp_TRIGGER(val);
		//DEBUGMSG(TRUE, (TEXT("config PinspPara %d\r\n"),val));
		if(bSend)
		{
			getModel()->Send_PARA_PINSP(val,true,true);//newVG
		}
	}
}

/**********************************************************************************************//**
 * Sets pinsp para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPINSPPara_IPPV(SHORT val, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	//m_data.m_iParaDataPInsp=(val/5)*5;
	m_data.m_iParaDataPInsp_IPPV=val;

	//DEBUGMSG(TRUE, (TEXT("temp PinspPara %d\r\n"),val));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPInsp_IPPV(val);
		//DEBUGMSG(TRUE, (TEXT("config PinspPara %d\r\n"),val));
		if(bSend)
		{
			getModel()->Send_PARA_PINSP(val,true,true);//newVG
		}
	}
#else
	//m_data.m_iParaDataPInsp=(val/5)*5;
	m_data.m_iParaDataPInsp_TRIGGER=val;

	//DEBUGMSG(TRUE, (TEXT("temp PinspPara %d\r\n"),val));

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPInsp_TRIGGER(val);
		//DEBUGMSG(TRUE, (TEXT("config PinspPara %d\r\n"),val));
		if(bSend)
		{
			getModel()->Send_PARA_PINSP(val,true,true);//newVG
		}
	}
#endif
	
}

/**********************************************************************************************//**
 * Gets ppsv para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The ppsv para.
 **************************************************************************************************/

SHORT CParaData::GetPpsvPara()
{
	return m_data.m_iParaDataPpsv;
}

/**********************************************************************************************//**
 * Sets ppsv para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPpsvPara(SHORT value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataPpsv=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPpsv(value);
		if(bSend)
		{
			//DEBUGMSG(TRUE, (TEXT("send PPSV %d\r\n"),value));
			getModel()->Send_PARA_P_PSV(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets o 2 para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The o 2 para.
 **************************************************************************************************/

BYTE CParaData::GetO2Para()
{
	return m_data.m_iParaDataO2;
}

/**********************************************************************************************//**
 * Sets o 2 para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetO2Para(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataO2=value;

	if(bConfig)
	{
		//DEBUGMSG(TRUE, (TEXT("CParaData::SetO2Para config %d\r\n"),value));
		getModel()->getCONFIG()->SetParaDataO2(value);
		if(bSend && getModel()->isO2FlushActive()==false)
		{
			getModel()->Send_PARA_OXY_RATIO(value,true,true);
		}
	}
	
}

/**********************************************************************************************//**
 * Gets o 2 flush para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The o 2 flush para.
 **************************************************************************************************/

BYTE CParaData::GetO2FlushPara()
{
	return m_data.m_iParaDataO2Flush;
}

/**********************************************************************************************//**
 * Sets o 2 flush para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetO2FlushPara(BYTE value, bool bConfig)
{
	m_data.m_iParaDataO2Flush=value;

	//wird erst bei Aktivierung des Flush an Schnittstelle geschickt
	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataO2Flush(value);
		if(getModel()->isO2FlushActive())
		{
			getModel()->Send_PARA_OXY_RATIO(value,true,true);
		}

		/*BYTE iO2Difference=value-getModel()->getDATAHANDLER()->GetCurrentO2Para();
		if(iO2Difference<MAXDIFF_O2_FLUSH)
			iO2Difference=MAXDIFF_O2_FLUSH;*/
	}
	
}

/**********************************************************************************************//**
 * Gets trigger para convert
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger para convert.
 **************************************************************************************************/

BYTE CParaData::GetTriggerPara_CONV()
{
	return m_data.m_iParaDataTrigger_CONV;
}

/**********************************************************************************************//**
 * Sets trigger para convert
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetTriggerPara_CONV(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataTrigger_CONV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataTrigger_CONV(value);
		if(bSend)
		{
			getModel()->Send_PARA_TRIG_SCHWELLE(value,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets trigger para cpap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger para cpap.
 **************************************************************************************************/

BYTE CParaData::GetTriggerPara_CPAP()
{
	return m_data.m_iParaDataTrigger_CPAP;
}

/**********************************************************************************************//**
 * Sets trigger para cpap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetTriggerPara_CPAP(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataTrigger_CPAP=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataTrigger_CPAP(value);
		if(bSend)
		{
			getModel()->Send_PARA_TRIG_SCHWELLE(value,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets trigger para duopap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger para duopap.
 **************************************************************************************************/

BYTE CParaData::GetTriggerPara_DUOPAP()
{
	return m_data.m_iParaDataTrigger_DUOPAP;
}

/**********************************************************************************************//**
 * Sets trigger para duopap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetTriggerPara_DUOPAP(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataTrigger_DUOPAP=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataTrigger_DUOPAP(value);
		if(bSend)
		{
			getModel()->Send_PARA_TRIG_SCHWELLE(value,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets trigger para ncpap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger para ncpap.
 **************************************************************************************************/

BYTE CParaData::GetTriggerPara_NCPAP()
{
	return m_data.m_iParaDataTrigger_NCPAP;
}

/**********************************************************************************************//**
 * Sets trigger para ncpap
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetTriggerPara_NCPAP(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataTrigger_NCPAP=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataTrigger_NCPAP(value);
		if(bSend)
		{
			getModel()->Send_PARA_TRIG_SCHWELLE(value,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets eflow para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The eflow para trigger.
 **************************************************************************************************/

WORD CParaData::GetEFLOWPara_TRIGGER()
{
	return m_data.m_iParaDataEFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Sets eflow para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetEFLOWPara_TRIGGER(WORD value, bool bSend, bool bConfig)
{
	//DEBUGMSG(TRUE, (TEXT("set SetEFLOWPara_TRIGGER %d\r\n"),value));
	m_data.m_iParaDataEFlow_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataEFlow_TRIGGER(value);
		if(bSend)
		{
			//DEBUGMSG(TRUE, (TEXT("send SetEFLOWPara_TRIGGER %d\r\n"),value));
			getModel()->Send_PARA_EXH_FLOW(value,true,true);
		}
	}
	
}

/**********************************************************************************************//**
 * Gets eflow para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The eflow para ippv.
 **************************************************************************************************/

WORD CParaData::GetEFLOWPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataEFlow_IPPV;
#else
	return m_data.m_iParaDataEFlow_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets eflow para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetEFLOWPara_IPPV(WORD value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataEFlow_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataEFlow_IPPV(value);
		if(bSend)
		{
			getModel()->Send_PARA_EXH_FLOW(value,true,true);
		}
	}
#else
	//DEBUGMSG(TRUE, (TEXT("set SetEFLOWPara_IPPV %d\r\n"),value));
	m_data.m_iParaDataEFlow_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataEFlow_TRIGGER(value);
		if(bSend)
		{
			//DEBUGMSG(TRUE, (TEXT("send SetEFLOWPara_IPPV %d\r\n"),value));
			getModel()->Send_PARA_EXH_FLOW(value,true,true);
		}
	}
#endif
	

}

/**********************************************************************************************//**
 * Gets therapie flow para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The therapie flow para.
 **************************************************************************************************/

WORD CParaData::GetTherapieFLOWPara()
{
	return m_data.m_iParaDataTherapieFlow;
}

/**********************************************************************************************//**
 * Sets therapie flow para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetTherapieFLOWPara(WORD value, bool bSend, bool bConfig)
{
	DEBUGMSG(TRUE, (TEXT("set SetTherapieFLOWPara %d\r\n"),value));
	m_data.m_iParaDataTherapieFlow=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataTherapieFlow(value);
		if(bSend)
		{
			DEBUGMSG(TRUE, (TEXT("send SetTherapieFLOWPara %d\r\n"),value));
			getModel()->Send_PARA_EXH_FLOW(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets flowmin para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The flowmin para.
 **************************************************************************************************/

WORD CParaData::GetFlowminPara()
{
	return m_data.m_iParaDataFlowMin;
}

/**********************************************************************************************//**
 * Sets flowmin para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetFlowminPara(WORD value, bool bSend, bool bConfig)
{
	DEBUGMSG(TRUE, (TEXT("set SetFlowminPara %d\r\n"),value));
	m_data.m_iParaDataFlowMin=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataFlowMin(value);
		if(bSend)
		{
			DEBUGMSG(TRUE, (TEXT("send SetFlowminPara %d\r\n"),value));
			getModel()->Send_PARA_EXH_FLOW(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets peep para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPEEPPara_TRIGGER()
{
	return m_data.m_iParaDataPEEP_TRIGGER;
}

/**********************************************************************************************//**
 * Sets peep para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPEEPPara_TRIGGER(SHORT value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataPEEP_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPEEP_TRIGGER(value);
		if(bSend)
		{
			getModel()->Send_PARA_PEEP(value,true,true);
		}
	}
	
}

/**********************************************************************************************//**
 * Gets peep para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPEEPPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataPEEP_IPPV;
#else
	return m_data.m_iParaDataPEEP_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets peep para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetPEEPPara_IPPV(SHORT value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataPEEP_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPEEP_IPPV(value);
		if(bSend)
		{
			getModel()->Send_PARA_PEEP(value,true,true);
		}
	}
#else
	m_data.m_iParaDataPEEP_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataPEEP_TRIGGER(value);
		if(bSend)
		{
			getModel()->Send_PARA_PEEP(value,true,true);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Gets bpm para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpm para ippv.
 **************************************************************************************************/

WORD CParaData::GetBPMPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataBPM_IPPV;
#else
	return m_data.m_iParaDataBPM_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets bpm para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetBPMPara_IPPV(WORD value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataBPM_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataBPM_IPPV(value);
		if(bSend)
		{
			if(getModel()->getAcuLink()!=NULL)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,value);
		}
	}
#else
	m_data.m_iParaDataBPM_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataBPM_TRIGGER(value);
		if(bSend)
		{
			if(getModel()->getAcuLink()!=NULL)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,value);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Gets bpm para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpm para trigger.
 **************************************************************************************************/

WORD CParaData::GetBPMPara_TRIGGER()
{
	return m_data.m_iParaDataBPM_TRIGGER;
}

/**********************************************************************************************//**
 * Sets bpm para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetBPMPara_TRIGGER(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataBPM_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataBPM_TRIGGER(value);
		if(bSend)
		{
			if(getModel()->getAcuLink()!=NULL)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,value);
		}
	}

}

/**********************************************************************************************//**
 * Gets bpmnmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpmnmode para.
 **************************************************************************************************/

WORD CParaData::GetBPMNMODEPara()
{
	return m_data.m_iParaDataBPM_NMODE;
}

/**********************************************************************************************//**
 * Sets bpmnmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetBPMNMODEPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataBPM_NMODE=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataBPM_NMODE(value);
		if(bSend)
		{
			if(getModel()->getAcuLink()!=NULL)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_FREQ,value);
		}
	}

}

/**********************************************************************************************//**
 * Gets hf frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf frequency para.
 **************************************************************************************************/

BYTE CParaData::GetHFFreqPara()
{
	return m_data.m_iParaDataHFFreq;
}

/**********************************************************************************************//**
 * Sets hf frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFFreqPara(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataHFFreq=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFFreq(value);
		if(bSend)
		{
			getModel()->Send_PARA_HF_FREQ(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets hfampl para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfampl para.
 **************************************************************************************************/

WORD CParaData::GetHFAMPLPara()
{
	return m_data.m_iParaDataHFAMPL;
}

/**********************************************************************************************//**
 * Sets hfampl para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFAMPLPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataHFAMPL=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFAmpl(value);
		if(bSend)
		{
			getModel()->Send_PARA_HF_AMPL(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets hfamp lmax para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfamp lmax para.
 **************************************************************************************************/

WORD CParaData::GetHFAMPLmaxPara()
{
	return m_data.m_iParaDataHFAMPLmax;
}

/**********************************************************************************************//**
 * Sets hfamp lmax para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFAMPLmaxPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataHFAMPLmax=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFAmplmax(value);
		if(bSend)
		{
			getModel()->Send_PARA_HF_AMPL(value,true,true);
		}
	}
	
}

/**********************************************************************************************//**
 * Gets IE ratio para hfo
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The IE ratio para hfo.
 **************************************************************************************************/

eRatioIE CParaData::GetIERatioParaHFO()
{
	return m_data.m_iParaDataIERatio;
}

/**********************************************************************************************//**
 * Sets IE ratio para hfo
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	ratio  	The ratio.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetIERatioParaHFO(eRatioIE ratio, bool bSend, bool bConfig)
{
	m_data.m_iParaDataIERatio=ratio;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFIERatio(ratio);
		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
		}
	}

}

/**********************************************************************************************//**
 * Gets hfp mean para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean para.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanPara()
{
	return m_data.m_iParaDataHFPmean;
}

/**********************************************************************************************//**
 * Sets hfp mean para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFPMeanPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataHFPmean=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFPmean(value);
		if(bSend)
		{
			getModel()->Send_PARA_HF_PMITT(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets i time record para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time record para.
 **************************************************************************************************/

WORD CParaData::GetITimeRecPara()
{
	return m_data.m_iParaDataITIME_REC;
}

/**********************************************************************************************//**
 * Sets i time record para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetITimeRecPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataITIME_REC=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataITimeRec(value);
		if(bSend)
		{
			getModel()->Send_PARA_HF_ITIME_REC(value,true,false);
			getModel()->Send_PARA_HF_FREQ_REC(getModel()->getDATAHANDLER()->GetCurrentFreqRecPara(),true,false);
		}
	}

}

/**********************************************************************************************//**
 * Gets frequency record para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The frequency record para.
 **************************************************************************************************/

WORD CParaData::GetFreqRecPara()
{
	return m_data.m_iParaDataFREQ_REC;
}

/**********************************************************************************************//**
 * Sets frequency record para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetFreqRecPara(WORD value, bool bSend, bool bConfig)
{
	//if(value==0 && bSend)//PMAN1
	//{
	//	if(GetPManualHFOPara()<GetHFPMeanPara()+10)
	//	{
	//		SetPManualHFOPara(GetHFPMeanPara()+10, true, true);;
	//	}
	//}
	m_data.m_iParaDataFREQ_REC=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataFREQ_REC(value);
		if(bSend)
		{
			getModel()->Send_MODE_OPTION2();
			getModel()->Send_PARA_HF_FREQ_REC(value,true,false);

			getModel()->Send_PARA_HF_ITIME_REC(getModel()->getDATAHANDLER()->GetCurrentITimeRecPara(),true,false);
		}
	}

}

/**********************************************************************************************//**
 * Gets hfp mean record para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean record para.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanRecPara()
{
	return m_data.m_iParaDataPMEAN_REC;
}

/**********************************************************************************************//**
 * Sets hfp mean record para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFPMeanRecPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataPMEAN_REC=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFPMeanRec(value);
		if(bSend)
		{
			DEBUGMSG(TRUE, (TEXT("send PMeanRec %d\r\n"),value));
			getModel()->Send_PARA_HF_PMEANREC(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets i time para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time para trigger.
 **************************************************************************************************/

WORD CParaData::GetITimePara_TRIGGER()
{
	return m_data.m_iParaDataITime_TRIGGER;
}

/**********************************************************************************************//**
 * Sets i time para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetITimePara_TRIGGER(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataITime_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataITime_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_TIME(value,true,true);
		}
	}	
}

/**********************************************************************************************//**
 * Gets i time para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time para ippv.
 **************************************************************************************************/

WORD CParaData::GetITimePara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataITime_IPPV;
#else
	return m_data.m_iParaDataITime_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets i time para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetITimePara_IPPV(WORD value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataITime_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataITime_IPPV(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_TIME(value,true,true);
		}
	}
#else
	m_data.m_iParaDataITime_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataITime_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_TIME(value,true,true);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Gets i time nmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time nmode para.
 **************************************************************************************************/

WORD CParaData::GetITimeNMODEPara()
{
	return m_data.m_iParaDataITime_NMODE;
}

/**********************************************************************************************//**
 * Sets i time nmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetITimeNMODEPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataITime_NMODE=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataITime_NMODE(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_TIME(value,true,true);
		}
	}


}

/**********************************************************************************************//**
 * Gets etime para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etime para trigger.
 **************************************************************************************************/

WORD CParaData::GetETIMEPara_TRIGGER()
{
	return m_data.m_iParaDataETime_TRIGGER;
}

/**********************************************************************************************//**
 * Sets etime para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetETIMEPara_TRIGGER(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataETime_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataETIME_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_EXH_TIME(value,true,true);
		}
	}
	
}

/**********************************************************************************************//**
 * Gets etime para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etime para ippv.
 **************************************************************************************************/

WORD CParaData::GetETIMEPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataETime_IPPV;
#else
	return m_data.m_iParaDataETime_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets etime para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetETIMEPara_IPPV(WORD value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataETime_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataETIME_IPPV(value);

		if(bSend)
		{
			getModel()->Send_PARA_EXH_TIME(value,true,true);
		}
	}
#else
	m_data.m_iParaDataETime_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataETIME_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_EXH_TIME(value,true,true);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Gets etimenmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etimenmode para.
 **************************************************************************************************/

WORD CParaData::GetETIMENMODEPara()
{
	return m_data.m_iParaDataETime_NMODE;
}

/**********************************************************************************************//**
 * Sets etimenmode para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetETIMENMODEPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataETime_NMODE=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataETIME_NMODE(value);

		if(bSend)
		{
			getModel()->Send_PARA_EXH_TIME(value,true,true);
		}
	}

}

/**********************************************************************************************//**
 * Gets i flow para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow para trigger.
 **************************************************************************************************/

WORD CParaData::GetIFlowPara_TRIGGER()
{
	return m_data.m_iParaDataIFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Sets i flow para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetIFlowPara_TRIGGER(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataIFlow_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataIFlow_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_FLOW(value,true,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets i flow para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow para ippv.
 **************************************************************************************************/

WORD CParaData::GetIFlowPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataIFlow_IPPV;
#else
	return m_data.m_iParaDataIFlow_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets i flow para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetIFlowPara_IPPV(WORD value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataIFlow_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataIFlow_IPPV(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_FLOW(value,true,true);
		}
	}
#else
	m_data.m_iParaDataIFlow_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataIFlow_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_INSP_FLOW(value,true,true);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Gets hf flow para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf flow para.
 **************************************************************************************************/

WORD CParaData::GetHFFlowPara()
{
	return m_data.m_iParaDataHFFlow;
}

/**********************************************************************************************//**
 * Sets hf flow para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFFlowPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataHFFlow=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFFlow(value);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION2();
			
			getModel()->Send_PARA_INSP_FLOW(value,true,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets risetime para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The risetime para trigger.
 **************************************************************************************************/

WORD CParaData::GetRisetimePara_TRIGGER()
{
	return m_data.m_iParaDataRisetime_TRIGGER;
}

/**********************************************************************************************//**
 * Sets risetime para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetRisetimePara_TRIGGER(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataRisetime_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataRisetime_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_RISETIME(value,true,true);
		}
	}
}

/**********************************************************************************************//**
 * Gets risetime para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The risetime para ippv.
 **************************************************************************************************/

WORD CParaData::GetRisetimePara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataRisetime_IPPV;
#else
	return m_data.m_iParaDataRisetime_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Sets risetime para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetRisetimePara_IPPV(WORD value, bool bSend, bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_iParaDataRisetime_IPPV=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataRisetime_IPPV(value);

		if(bSend)
		{
			getModel()->Send_PARA_RISETIME(value,true,true);
		}
	}
#else
	m_data.m_iParaDataRisetime_TRIGGER=value;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataRisetime_TRIGGER(value);

		if(bSend)
		{
			getModel()->Send_PARA_RISETIME(value,true,true);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Gets v limit parameter trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v limit parameter trigger.
 **************************************************************************************************/

WORD CParaData::GetVLimitParam_TRIGGER()
{
	return m_data.m_iParaDataVLimit_TRIGGER;
}

/**********************************************************************************************//**
 * Gets v limit parameter ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v limit parameter ippv.
 **************************************************************************************************/

WORD CParaData::GetVLimitParam_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataVLimit_IPPV;
#else
	return m_data.m_iParaDataVLimit_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets v limit parameter trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bOn	   	True to on.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetVLimitParam_TRIGGER(WORD val, bool bOn ,bool bSend, bool bConfig)
{
	bool bStateChanged=false;
	if(m_data.m_bParaDataVLimitOn_TRIGGER!=bOn)
		bStateChanged=true;

	m_data.m_bParaDataVLimitOn_TRIGGER=bOn;
	m_data.m_iParaDataVLimit_TRIGGER=val;

	if(bStateChanged && bOn)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_ENABLED);
	}
	else if(bStateChanged && !bOn)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_DISABLED);
	}

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVLimitState_TRIGGER(bOn);
		getModel()->getCONFIG()->SetParaDataVLimit_TRIGGER(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_LIMIT(m_data.m_iParaDataVLimit_TRIGGER,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Sets v limit parameter ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bOn	   	True to on.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetVLimitParam_IPPV(WORD val, bool bOn ,bool bSend, bool bConfig)
{
	bool bStateChanged=false;

#ifdef BACKUP_VENTILATION
	if(m_data.m_bParaDataVLimitOn_IPPV!=bOn)
		bStateChanged=true;

	m_data.m_bParaDataVLimitOn_IPPV=bOn;
	m_data.m_iParaDataVLimit_IPPV=val;

	if(bStateChanged && bOn)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_ENABLED);
	}
	else if(bStateChanged && !bOn)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_DISABLED);
	}

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVLimitState_IPPV(bOn);
		getModel()->getCONFIG()->SetParaDataVLimit_IPPV(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_LIMIT(m_data.m_iParaDataVLimit_IPPV,false,true);
		}
	}
#else
	if(m_data.m_bParaDataVLimitOn_TRIGGER!=bOn)
		bStateChanged=true;

	m_data.m_bParaDataVLimitOn_TRIGGER=bOn;
	m_data.m_iParaDataVLimit_TRIGGER=val;

	if(bStateChanged && bOn)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_ENABLED);
	}
	else if(bStateChanged && !bOn)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_DISABLED);
	}

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVLimitState_TRIGGER(bOn);
		getModel()->getCONFIG()->SetParaDataVLimit_TRIGGER(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_LIMIT(m_data.m_iParaDataVLimit_TRIGGER,false,true);
		}
	}
#endif
}

/**********************************************************************************************//**
 * Query if this instance is v limit parameter on trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if v limit parameter on trigger, false if not.
 **************************************************************************************************/

bool CParaData::IsVLimitParamOn_TRIGGER()
{
	return m_data.m_bParaDataVLimitOn_TRIGGER;
}

/**********************************************************************************************//**
 * Query if this instance is v limit parameter on ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if v limit parameter on ippv, false if not.
 **************************************************************************************************/

bool CParaData::IsVLimitParamOn_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_bParaDataVLimitOn_IPPV;
#else
	return m_data.m_bParaDataVLimitOn_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets v garant para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v garant para trigger.
 **************************************************************************************************/

WORD CParaData::GetVGarantPara_TRIGGER()
{
	return m_data.m_iParaDataVGarant_TRIGGER;
}

/**********************************************************************************************//**
 * Gets v garant para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v garant para ippv.
 **************************************************************************************************/

WORD CParaData::GetVGarantPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_iParaDataVGarant_IPPV;
#else
	return m_data.m_iParaDataVGarant_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Sets v garant state trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bOn	   	True to on.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetVGarantState_TRIGGER(bool bOn,bool bSend,bool bConfig)
{
	m_data.m_bParaDataVGarantOn_TRIGGER=bOn;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVGarantState_TRIGGER(bOn);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_TRIGGER,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Sets v garant para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetVGarantPara_TRIGGER(WORD val,bool bSend,bool bConfig)
{
	m_data.m_iParaDataVGarant_TRIGGER=val;
	
	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVGarant_TRIGGER(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_TRIGGER,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Sets v garant state ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bOn	   	True to on.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetVGarantState_IPPV(bool bOn,bool bSend,bool bConfig)
{
#ifdef BACKUP_VENTILATION
	m_data.m_bParaDataVGarantOn_IPPV=bOn;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVGarantState_IPPV(bOn);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_IPPV,false,true);
		}
	}
#else
	m_data.m_bParaDataVGarantOn_TRIGGER=bOn;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVGarantState_TRIGGER(bOn);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_TRIGGER,false,true);
		}
	}

#endif
}

/**********************************************************************************************//**
 * Sets v garant para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetVGarantPara_IPPV(WORD val,bool bSend,bool bConfig)
{
	DEBUGMSG(TRUE, (TEXT("SetVGarantParam IPPV\r\n")));

#ifdef BACKUP_VENTILATION
	/*if(m_data.m_bParaDataVGarantOn_IPPV!=bOn)
		bStateChanged=true;

	m_data.m_bParaDataVGarantOn_IPPV=bOn;*/

	m_data.m_iParaDataVGarant_IPPV=val;

	if(bConfig)
	{
		//getModel()->getCONFIG()->SetParaDataVGarantState_IPPV(bOn);
		getModel()->getCONFIG()->SetParaDataVGarant_IPPV(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_IPPV,false,true);

		}
	}
#else
	m_data.m_iParaDataVGarant_TRIGGER=val;

	

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataVGarant_TRIGGER(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_TRIGGER,false,true);

		}
	}
#endif
}

/**********************************************************************************************//**
 * Query if this instance is v garant state on trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if v garant state on trigger, false if not.
 **************************************************************************************************/

bool CParaData::IsVGarantStateOn_TRIGGER()
{
	return m_data.m_bParaDataVGarantOn_TRIGGER;
}

/**********************************************************************************************//**
 * Query if this instance is v garant state on ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if v garant state on ippv, false if not.
 **************************************************************************************************/

bool CParaData::IsVGarantStateOn_IPPV()
{
#ifdef BACKUP_VENTILATION
	return m_data.m_bParaDataVGarantOn_IPPV;
#else
	return m_data.m_bParaDataVGarantOn_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets hfv garant para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfv garant para.
 **************************************************************************************************/

WORD CParaData::GetHFVGarantPara()
{
	return m_data.m_iParaDataVGarant_HFO;
}

/**********************************************************************************************//**
 * Sets hfv garant state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bOn	   	True to on.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFVGarantState(bool bOn,bool bSend,bool bConfig)
{
	m_data.m_bParaDataVGarantOn_HFO=bOn;

	if(bConfig)
	{
		getModel()->getCONFIG()->SetParaDataHFVGarantState(bOn);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_HFO,false,true);
		}
	}
}

/**********************************************************************************************//**
 * Sets hfv garant para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	val	   	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::SetHFVGarantPara(WORD val,bool bSend,bool bConfig)
{
	/*bool bStateChanged=false;
	if(m_data.m_bParaDataVGarantOn_HFO!=bOn)
		bStateChanged=true;

	m_data.m_bParaDataVGarantOn_HFO=bOn;*/

	m_data.m_iParaDataVGarant_HFO=val;

	//if(bStateChanged && !bPreset)
	//{
	//	if(bOn)
	//	{
	//		if(!bSend && !bAutoOff && !bAutoOn && bChangeState)
	//		{
	//			getModel()->getDATAHANDLER()->ChangeVgarantState(VGAR_PRESET_ON);
	//		}
	//	}
	//	else //if(!bOn)
	//	{
	//		if(bSend && bChangeState)
	//		{
	//			if(bAutoOff)//autovgarant
	//				getModel()->getDATAHANDLER()->ChangeVgarantState(VGAR_PARA_AUTODISABLED);
	//			else
	//				getModel()->getDATAHANDLER()->ChangeVgarantState(VGAR_OFF);
	//		}
	//		else if(!bAutoOff && !bAutoOn && bChangeState)
	//		{
	//			getModel()->getDATAHANDLER()->ChangeVgarantState(VGAR_PRESET_OFF);
	//		}
	//	}
	//}


	if(bConfig)
	{
		//getModel()->getCONFIG()->SetParaDataHFVGarantState(bOn);
		getModel()->getCONFIG()->SetParaDataHFVGarant(val);

		if(bSend)
		{
			getModel()->Send_MODE_OPTION1();
			getModel()->Send_PARA_VOLUME_GARANT(m_data.m_iParaDataVGarant_HFO,false,true);

			/*if(bChangeState && !bOn && !bAutoOff)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VGAR_OFF);
			}
			else if(bOn)
			{
				if(!bAutoOn && bChangeState)
					getModel()->getDATAHANDLER()->ChangeVgarantState(VGAR_ON);

			}*/

		}
	}
}

/**********************************************************************************************//**
 * Query if this instance is hfv garant state on
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if hfv garant state on, false if not.
 **************************************************************************************************/

bool CParaData::IsHFVGarantStateOn()
{
	return m_data.m_bParaDataVGarantOn_HFO;
}

/**********************************************************************************************//**
 * Gets byte fo tconv amplitude para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The byte fo tconv amplitude para.
 **************************************************************************************************/

BYTE CParaData::getByteFOTconv_AMPLITUDEPara()
{
	return m_byFOTAmp;
}

/**********************************************************************************************//**
 * Gets fo tconv amplitude para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv amplitude para.
 **************************************************************************************************/

WORD CParaData::getFOTconv_AMPLITUDEPara()
{
	return m_data.m_iParaDataFOTconv_AMPLITUDE;
}

/**********************************************************************************************//**
 * Sets fo tconv amplitude para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	ampl   	The ampl.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOTconv_AMPLITUDEPara(WORD ampl, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOTconv_AMPLITUDE=ampl;
	switch(ampl)
	{
	/*case 0:
		{
			m_byFOTAmp=0;
		}
		break;*/
	case 25:
		{
			m_byFOTAmp=1;
		}
		break;
	case 50:
		{
			m_byFOTAmp=2;
		}
		break;
	case 75:
		{
			m_byFOTAmp=3;
		}
		break;
	default:
		{
			m_byFOTAmp=0;
		}
		break;
	}

	if(bConfig)
	{
		getModel()->getCONFIG()->setFOTconv_AMPLITUDE(ampl);

		/*if(bSend)
		{
			if(getModel()->getDATAHANDLER()->getFOTstate())
			{
				getModel()->getSERIAL()->Send_FOTconv_AMP_FREQ(iRes);
			}
			else
			{
				getModel()->getSERIAL()->Send_FOTconv_AMP_FREQ(0);
			}
		}*/
	}
}

/**********************************************************************************************//**
 * Gets byte fo tconv frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The byte fo tconv frequency para.
 **************************************************************************************************/

BYTE CParaData::getByteFOTconv_FREQPara()
{
	return m_byFOTFreq;
}

/**********************************************************************************************//**
 * Gets fo tconv frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv frequency para.
 **************************************************************************************************/

BYTE CParaData::getFOTconv_FREQPara()
{
	return m_data.m_iParaDataFOTconv_FREQ;
}

/**********************************************************************************************//**
 * Sets fo tconv frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOTconv_FREQPara(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOTconv_FREQ=value;
	switch(value)
	{
	case 5:
		{
			m_byFOTFreq=0;
		}
		break;
	case 10:
		{
			m_byFOTFreq=4;
		}
		break;
	case 15:
		{
			m_byFOTFreq=8;
		}
		break;
	default:
		{
			m_byFOTFreq=0;
		}
		break;
	}

	if(bConfig)
	{
		getModel()->getCONFIG()->setFOTconv_FREQ(value);

		/*if(bSend)
		{
			if(getModel()->getDATAHANDLER()->getFOTstate())
			{
				getModel()->getSERIAL()->Send_FOTconv_AMP_FREQ(iRes);
			}
			else
			{
				getModel()->getSERIAL()->Send_FOTconv_AMP_FREQ(0);
			}
		}*/
		
	}
}

/**********************************************************************************************//**
 * Gets fo tconv peepstart para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv peepstart para.
 **************************************************************************************************/

WORD CParaData::getFOTconv_PEEPSTARTPara()
{
	return m_data.m_iParaDataFOTconv_PEEPSTART;
}

/**********************************************************************************************//**
 * Sets fo tconv peepstart para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOTconv_PEEPSTARTPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOTconv_PEEPSTART=value;
	if(bConfig)
	{
		getModel()->getCONFIG()->setFOTconv_PEEPSTART(value);

		/*if(bSend)
		{
			
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo tconv peepend para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv peepend para.
 **************************************************************************************************/

WORD CParaData::getFOTconv_PEEPENDPara()
{
	return m_data.m_iParaDataFOTconv_PEEPEND;
}

/**********************************************************************************************//**
 * Sets fo tconv peepend para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOTconv_PEEPENDPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOTconv_PEEPEND=value;
	if(bConfig)
	{
		getModel()->getCONFIG()->setFOTconv_PEEPEND(value);

		/*if(bSend)
		{
			
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo tconv steps para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv steps para.
 **************************************************************************************************/

BYTE CParaData::getFOTconv_STEPSPara()
{
	return m_data.m_iParaDataFOTconv_STEPS;
}

/**********************************************************************************************//**
 * Sets fo tconv steps para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOTconv_STEPSPara(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOTconv_STEPS=value;
	if(bConfig)
	{
		WORD iDiff=value/2;
		getModel()->getCONFIG()->setDiffPEEPFOT(iDiff*10);
		getModel()->getCONFIG()->setFOTconv_STEPS(value);

		/*if(bSend)
		{
			
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo thfo amplitude para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo amplitude para.
 **************************************************************************************************/

WORD CParaData::getFOThfo_AMPLITUDEPara()
{
	return m_data.m_iParaDataFOThfo_AMPLITUDE;
}

/**********************************************************************************************//**
 * Sets fo thfo amplitude para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	ampl   	The ampl.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOThfo_AMPLITUDEPara(WORD ampl, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOThfo_AMPLITUDE=ampl;


	if(bConfig)
	{
		getModel()->getCONFIG()->setFOThfo_AMPLITUDE(ampl);

		/*if(bSend)
		{
			getModel()->Send_PARA_HF_AMPL(ampl,true,true);
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo thfo frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo frequency para.
 **************************************************************************************************/

BYTE CParaData::getFOThfo_FREQPara()
{
	return m_data.m_iParaDataFOThfo_FREQ;
}

/**********************************************************************************************//**
 * Sets fo thfo frequency para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOThfo_FREQPara(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOThfo_FREQ=value;
	if(bConfig)
	{
		getModel()->getCONFIG()->setFOThfo_FREQ(value);

		/*if(bSend)
		{
			getModel()->Send_PARA_HF_FREQ(value,true,true);
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo thfo pmeanstart para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo pmeanstart para.
 **************************************************************************************************/

WORD CParaData::getFOThfo_PMEANSTARTPara()
{
	return m_data.m_iParaDataFOThfo_PMEANSTART;
}

/**********************************************************************************************//**
 * Sets fo thfo pmeanstart para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOThfo_PMEANSTARTPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOThfo_PMEANSTART=value;
	if(bConfig)
	{
		getModel()->getCONFIG()->setFOThfo_PMEANSTART(value);

		/*if(bSend)
		{
			
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo thfo pmeanend para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo pmeanend para.
 **************************************************************************************************/

WORD CParaData::getFOThfo_PMEANENDPara()
{
	return m_data.m_iParaDataFOThfo_PMEANEND;
}

/**********************************************************************************************//**
 * Sets fo thfo pmeanend para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOThfo_PMEANENDPara(WORD value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOThfo_PMEANEND=value;
	if(bConfig)
	{
		getModel()->getCONFIG()->setFOThfo_PMEANEND(value);

		/*if(bSend)
		{
			
		}*/
	}
}

/**********************************************************************************************//**
 * Gets fo thfo steps para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo steps para.
 **************************************************************************************************/

BYTE CParaData::getFOThfo_STEPSPara()
{
	return m_data.m_iParaDataFOThfo_STEPS;
}

/**********************************************************************************************//**
 * Sets fo thfo steps para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	value  	The value.
 * \param	bSend  	True to send.
 * \param	bConfig	True to configuration.
 **************************************************************************************************/

void CParaData::setFOThfo_STEPSPara(BYTE value, bool bSend, bool bConfig)
{
	m_data.m_iParaDataFOThfo_STEPS=value;
	if(bConfig)
	{
		WORD iDiff=value/2;
		getModel()->getCONFIG()->setDiffPmeanFOT(iDiff*10);
		getModel()->getCONFIG()->setFOThfo_STEPS(value);

		/*if(bSend)
		{
			
		}*/
	}
}

/**********************************************************************************************//**
 * Gets backup maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The backup maximum para.
 **************************************************************************************************/

BYTE CParaData::GetBackupMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataBackup;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataBackup;
	
}

/**********************************************************************************************//**
 * Gets cpap maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpap maximum para.
 **************************************************************************************************/

SHORT CParaData::GetCPAPMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataCPAP;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataCPAP;
}

/**********************************************************************************************//**
 * Gets cpapnmode maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpapnmode maximum para.
 **************************************************************************************************/

SHORT CParaData::GetCPAPNMODEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataCPAP_NMODE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataCPAP_NMODE;
}

/**********************************************************************************************//**
 * Gets pinsp maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pinsp maximum para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPINSPMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPInsp_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPInsp_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Gets pinsp maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pinsp maximum para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPINSPMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPInsp_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER;
}

/**********************************************************************************************//**
 * Gets pmax maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pmax maximum para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPMAXMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER;
#endif

}

/**********************************************************************************************//**
 * Gets pmax maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pmax maximum para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPMAXMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER;
}

/**********************************************************************************************//**
 * Gets ppsv maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The ppsv maximum para.
 **************************************************************************************************/

SHORT CParaData::GetPpsvMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPpsv;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPpsv;
}

/**********************************************************************************************//**
 * Gets hf frequency maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf frequency maximum para.
 **************************************************************************************************/

BYTE CParaData::GetHFFreqMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataHFFreq;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataHFFreq;
}

/**********************************************************************************************//**
 * Gets hfampl maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfampl maximum para.
 **************************************************************************************************/

WORD CParaData::GetHFAMPLMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMaxRange_NEONATAL.m_iParaDataHFAMPL;
	}
	else
	{
		return m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPL;
	}
}

/**********************************************************************************************//**
 * Sets hf ampl maximum para neonatal
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CParaData::SetHFAmplMaxParaNEONATAL(WORD iVal)
{
	m_dataMaxRange_NEONATAL.m_iParaDataHFAMPL=iVal;
	m_dataMaxRange_NEONATAL.m_iParaDataHFAMPLmax=iVal;
}

/**********************************************************************************************//**
 * Sets hf ampl maximum para pediatric
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CParaData::SetHFAmplMaxParaPEDIATRIC(WORD iVal)
{
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPL=iVal;
	m_dataMaxRange_PEDIATRIC.m_iParaDataHFAMPLmax=iVal;
}

/**********************************************************************************************//**
 * Gets hfp mean maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean maximum para.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataHFPmean;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataHFPmean;
}

/**********************************************************************************************//**
 * Gets fo tconv amplitude maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv amplitude maximum para.
 **************************************************************************************************/

WORD CParaData::getFOTconv_AMPLITUDEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_AMPLITUDE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE;
}

/**********************************************************************************************//**
 * Gets fo tconv frequency maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv frequency maximum para.
 **************************************************************************************************/

BYTE CParaData::getFOTconv_FREQMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_FREQ;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_FREQ;
}

/**********************************************************************************************//**
 * Gets fo tconv steps maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv steps maximum para.
 **************************************************************************************************/

BYTE CParaData::getFOTconv_STEPSMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFOTconv_STEPS;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOTconv_STEPS;
}

/**********************************************************************************************//**
 * Gets fo thfo steps maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo steps maximum para.
 **************************************************************************************************/

BYTE CParaData::getFOThfo_STEPSMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_STEPS;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_STEPS;
}
//WORD CParaData::getFOThfo_PMEANSTARTMaxPara()
//{
//	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
//		return m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_PMEANSTART;
//	else
//		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART;
//}
//WORD CParaData::getFOThfo_PMEANENDMaxPara()
//{
//	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
//		return m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_PMEANEND;
//	else
//		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_PMEANEND;
//}

/**********************************************************************************************//**
 * Gets fo thfo amplitude maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo amplitude maximum para.
 **************************************************************************************************/

WORD CParaData::getFOThfo_AMPLITUDEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_AMPLITUDE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE;
}

/**********************************************************************************************//**
 * Gets fo thfo frequency maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo frequency maximum para.
 **************************************************************************************************/

BYTE CParaData::getFOThfo_FREQMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFOThfo_FREQ;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFOThfo_FREQ;
}

/**********************************************************************************************//**
 * Gets i time record maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time record maximum para.
 **************************************************************************************************/

WORD CParaData::GetITimeRecMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataITIME_REC;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataITIME_REC;
}

/**********************************************************************************************//**
 * Gets frequency record maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The frequency record maximum para.
 **************************************************************************************************/

WORD CParaData::GetFreqRecMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFREQ_REC;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFREQ_REC;
}

/**********************************************************************************************//**
 * Gets hfp mean record maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean record maximum para.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanRecMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPMEAN_REC;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPMEAN_REC;
}

/**********************************************************************************************//**
 * Gets IE ratio maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The IE ratio maximum para.
 **************************************************************************************************/

eRatioIE CParaData::GetIERatioMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataIERatio;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataIERatio;
}

/**********************************************************************************************//**
 * Gets p manual maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual maximum para.
 **************************************************************************************************/

SHORT CParaData::GetPManualMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPManual_HFO;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_HFO;
}

/**********************************************************************************************//**
 * Gets p manual nmode maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual nmode maximum para.
 **************************************************************************************************/

SHORT CParaData::GetPManualNMODEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPManual_NMODE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPManual_NMODE;
}

/**********************************************************************************************//**
 * Gets o 2 maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The o 2 maximum para.
 **************************************************************************************************/

BYTE CParaData::GetO2MaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataO2;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataO2;
}

/**********************************************************************************************//**
 * Gets o 2 flush maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The o 2 flush maximum para.
 **************************************************************************************************/

BYTE CParaData::GetO2FlushMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataO2Flush;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataO2Flush;
}

/**********************************************************************************************//**
 * Gets trigger convert maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger convert maximum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerCONVMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataTrigger_CONV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_CONV;
}

/**********************************************************************************************//**
 * Gets trigger cpap maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger cpap maximum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerCPAPMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataTrigger_CPAP;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_CPAP;
}

/**********************************************************************************************//**
 * Gets trigger duopap maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger duopap maximum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerDUOPAPMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataTrigger_DUOPAP;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_DUOPAP;
}

/**********************************************************************************************//**
 * Gets trigger ncpap maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger ncpap maximum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerNCPAPMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataTrigger_NCPAP;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataTrigger_NCPAP;
}

/**********************************************************************************************//**
 * Gets eflow maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The eflow maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetEFLOWMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataEFlow_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataEFlow_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets eflow maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The eflow maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetEFLOWMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataEFlow_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Gets therapie flow maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The therapie flow maximum para.
 **************************************************************************************************/

WORD CParaData::GetTherapieFlowMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataTherapieFlow;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataTherapieFlow;
}

/**********************************************************************************************//**
 * Gets flowmin maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The flowmin maximum para.
 **************************************************************************************************/

WORD CParaData::GetFlowminMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataFlowMin;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataFlowMin;
}

/**********************************************************************************************//**
 * Gets peep maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep maximum para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPEEP_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPEEP_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets peep maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep maximum para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataPEEP_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER;
}

/**********************************************************************************************//**
 * Gets bpm maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpm maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetBPMMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMaxRange_NEONATAL.m_iParaDataBPM_IPPV;
	}
	else
	{
		return m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_IPPV;
	}
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMaxRange_NEONATAL.m_iParaDataBPM_TRIGGER;
	}
	else
	{
		return m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_TRIGGER;
	}
#endif
}

/**********************************************************************************************//**
 * Gets bpm maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpm maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetBPMMaxPara_TRIGGER()
{
	bool bSIMVmode=false;
	if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
		bSIMVmode=true;

	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(bSIMVmode)
			return m_dataMaxRange_NEONATAL.m_iParaDataBPM_SIMV;
		else
			return m_dataMaxRange_NEONATAL.m_iParaDataBPM_TRIGGER;
	}
	else
	{
		if(bSIMVmode)
			return m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_SIMV;
		else
			return m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_TRIGGER;
	}
}

/**********************************************************************************************//**
 * Gets bpmnmode maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpmnmode maximum para.
 **************************************************************************************************/

WORD CParaData::GetBPMNMODEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataBPM_NMODE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataBPM_NMODE;
}

/**********************************************************************************************//**
 * Gets i time maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetITimeMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataITime_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataITime_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataITime_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataITime_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets i time maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetITimeMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataITime_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataITime_TRIGGER;
}

/**********************************************************************************************//**
 * Gets i time nmode maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time nmode maximum para.
 **************************************************************************************************/

WORD CParaData::GetITimeNMODEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataITime_NMODE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataITime_NMODE;
}

/**********************************************************************************************//**
 * Gets etime maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etime maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetETIMEMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMaxRange_NEONATAL.m_iParaDataETime_IPPV;
	}
	else
	{
		return m_dataMaxRange_PEDIATRIC.m_iParaDataETime_IPPV;
	}
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMaxRange_NEONATAL.m_iParaDataETime_TRIGGER;
	}
	else
	{
		return m_dataMaxRange_PEDIATRIC.m_iParaDataETime_TRIGGER;
	}
#endif
}

/**********************************************************************************************//**
 * Gets etime maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etime maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetETIMEMaxPara_TRIGGER()
{
	bool bSIMVmode=false;
	if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
		bSIMVmode=true;


	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(bSIMVmode)
			return m_dataMaxRange_NEONATAL.m_iParaDataETime_SIMV;
		else
			return m_dataMaxRange_NEONATAL.m_iParaDataETime_TRIGGER;
	}
	else
	{
		if(bSIMVmode)
			return m_dataMaxRange_PEDIATRIC.m_iParaDataETime_SIMV;
		else
			return m_dataMaxRange_PEDIATRIC.m_iParaDataETime_TRIGGER;
	}
}

/**********************************************************************************************//**
 * Gets etimenmode maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etimenmode maximum para.
 **************************************************************************************************/

WORD CParaData::GetETIMENMODEMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataETime_NMODE;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataETime_NMODE;
}

/**********************************************************************************************//**
 * Gets risetime maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The risetime maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetRisetimeMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataRisetime_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataRisetime_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets risetime maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The risetime maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetRisetimeMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataRisetime_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER;
}

/**********************************************************************************************//**
 * Gets i flow maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetIFlowMaxPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataIFlow_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataIFlow_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets i flow maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetIFlowMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataIFlow_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Gets hf flow maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf flow maximum para.
 **************************************************************************************************/

WORD CParaData::GetHFFlowMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataHFFlow;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataHFFlow;
}

/**********************************************************************************************//**
 * Gets v limit maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v limit maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetVLimitMaxPara_IPPV()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataVLimit_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataVLimit_IPPV;
}

/**********************************************************************************************//**
 * Gets v limit maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v limit maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetVLimitMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataVLimit_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataVLimit_TRIGGER;
}

/**********************************************************************************************//**
 * Gets v garant maximum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v garant maximum para ippv.
 **************************************************************************************************/

WORD CParaData::GetVGarantMaxPara_IPPV()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataVGarant_IPPV;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_IPPV;
}

/**********************************************************************************************//**
 * Gets v garant maximum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v garant maximum para trigger.
 **************************************************************************************************/

WORD CParaData::GetVGarantMaxPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataVGarant_TRIGGER;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_TRIGGER;
}

/**********************************************************************************************//**
 * Gets hfv garant maximum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfv garant maximum para.
 **************************************************************************************************/

WORD CParaData::GetHFVGarantMaxPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMaxRange_NEONATAL.m_iParaDataVGarant_HFO;
	else
		return m_dataMaxRange_PEDIATRIC.m_iParaDataVGarant_HFO;
}

/**********************************************************************************************//**
 * Gets backup minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The backup minimum para.
 **************************************************************************************************/

BYTE CParaData::GetBackupMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataBackup;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataBackup;
}

/**********************************************************************************************//**
 * Gets cpap minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpap minimum para.
 **************************************************************************************************/

SHORT CParaData::GetCPAPMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataCPAP;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataCPAP;
}

/**********************************************************************************************//**
 * Gets cpapnmode minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpapnmode minimum para.
 **************************************************************************************************/

SHORT CParaData::GetCPAPNMODEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataCPAP_NMODE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataCPAP_NMODE;
}

/**********************************************************************************************//**
 * Gets pinsp minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pinsp minimum para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPINSPMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPInsp_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPInsp_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets pinsp minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pinsp minimum para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPINSPMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPInsp_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPInsp_TRIGGER;
}

/**********************************************************************************************//**
 * Gets pmax minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pmax minimum para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPMAXMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets pmax minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The pmax minimum para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPMAXMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPmaxVolG_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPmaxVolG_TRIGGER;
}

/**********************************************************************************************//**
 * Gets ppsv minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The ppsv minimum para.
 **************************************************************************************************/

SHORT CParaData::GetPpsvMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPpsv;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPpsv;
}

/**********************************************************************************************//**
 * Gets hf frequency minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf frequency minimum para.
 **************************************************************************************************/

BYTE CParaData::GetHFFreqMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataHFFreq;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataHFFreq;
}

/**********************************************************************************************//**
 * Gets hfampl minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfampl minimum para.
 **************************************************************************************************/

WORD CParaData::GetHFAMPLMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataHFAMPL;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataHFAMPL;
}

/**********************************************************************************************//**
 * Gets hfp mean minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean minimum para.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataHFPmean;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataHFPmean;
}

/**********************************************************************************************//**
 * Gets fo tconv amplitude minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv amplitude minimum para.
 **************************************************************************************************/

WORD CParaData::getFOTconv_AMPLITUDEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFOTconv_AMPLITUDE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_AMPLITUDE;
}

/**********************************************************************************************//**
 * Gets fo tconv frequency minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv frequency minimum para.
 **************************************************************************************************/

BYTE CParaData::getFOTconv_FREQMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFOTconv_FREQ;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_FREQ;
}

/**********************************************************************************************//**
 * Gets fo tconv steps minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo tconv steps minimum para.
 **************************************************************************************************/

BYTE CParaData::getFOTconv_STEPSMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFOTconv_STEPS;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFOTconv_STEPS;
}

/**********************************************************************************************//**
 * Gets fo thfo steps minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo steps minimum para.
 **************************************************************************************************/

BYTE CParaData::getFOThfo_STEPSMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFOThfo_STEPS;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_STEPS;
}
//WORD CParaData::getFOThfo_PMEANSTARTMinPara()
//{
//	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
//		return m_dataMinRange_NEONATAL.m_iParaDataFOThfo_PMEANSTART;
//	else
//		return m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_PMEANSTART;
//}
//WORD CParaData::getFOThfo_PMEANENDMinPara()
//{
//	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
//		return m_dataMinRange_NEONATAL.m_iParaDataFOThfo_PMEANEND;
//	else
//		return m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_PMEANEND;
//}

/**********************************************************************************************//**
 * Gets fo thfo amplitude minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo amplitude minimum para.
 **************************************************************************************************/

WORD CParaData::getFOThfo_AMPLITUDEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFOThfo_AMPLITUDE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_AMPLITUDE;
}

/**********************************************************************************************//**
 * Gets fo thfo frequency minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The fo thfo frequency minimum para.
 **************************************************************************************************/

BYTE CParaData::getFOThfo_FREQMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFOThfo_FREQ;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFOThfo_FREQ;
}

/**********************************************************************************************//**
 * Gets i time record minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time record minimum para.
 **************************************************************************************************/

WORD CParaData::GetITimeRecMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataITIME_REC;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataITIME_REC;
}

/**********************************************************************************************//**
 * Gets frequency record minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The frequency record minimum para.
 **************************************************************************************************/

WORD CParaData::GetFreqRecMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFREQ_REC;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFREQ_REC;
}

/**********************************************************************************************//**
 * Gets hfp mean record minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean record minimum para.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanRecMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPMEAN_REC;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPMEAN_REC;
}

/**********************************************************************************************//**
 * Gets IE ratio minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The IE ratio minimum para.
 **************************************************************************************************/

eRatioIE CParaData::GetIERatioMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataIERatio;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataIERatio;
}

/**********************************************************************************************//**
 * Gets p manual minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual minimum para.
 **************************************************************************************************/

SHORT CParaData::GetPManualMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPManual_HFO;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPManual_HFO;
}

/**********************************************************************************************//**
 * Gets p manual nmode minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual nmode minimum para.
 **************************************************************************************************/

SHORT CParaData::GetPManualNMODEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPManual_NMODE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPManual_NMODE;
}

/**********************************************************************************************//**
 * Gets o 2 minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The o 2 minimum para.
 **************************************************************************************************/

BYTE CParaData::GetO2MinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataO2;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataO2;
}

/**********************************************************************************************//**
 * Gets o 2 flush minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The o 2 flush minimum para.
 **************************************************************************************************/

BYTE CParaData::GetO2FlushMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataO2Flush;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataO2Flush;
}

/**********************************************************************************************//**
 * Gets trigger convert minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger convert minimum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerCONVMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataTrigger_CONV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_CONV;
}

/**********************************************************************************************//**
 * Gets trigger cpap minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger cpap minimum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerCPAPMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataTrigger_CPAP;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_CPAP;
}

/**********************************************************************************************//**
 * Gets trigger duopap minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger duopap minimum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerDUOPAPMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataTrigger_DUOPAP;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_DUOPAP;
}

/**********************************************************************************************//**
 * Gets trigger ncpap minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The trigger ncpap minimum para.
 **************************************************************************************************/

BYTE CParaData::GetTriggerNCPAPMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataTrigger_NCPAP;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataTrigger_NCPAP;
}

/**********************************************************************************************//**
 * Gets therapie flow minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The therapie flow minimum para.
 **************************************************************************************************/

WORD CParaData::GetTherapieFlowMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataTherapieFlow;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataTherapieFlow;
}

/**********************************************************************************************//**
 * Gets eflow minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The eflow minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetEFLOWMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataEFlow_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataEFlow_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets eflow minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The eflow minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetEFLOWMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataEFlow_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataEFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Gets flowmin minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The flowmin minimum para.
 **************************************************************************************************/

WORD CParaData::GetFlowminMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataFlowMin;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataFlowMin;
}

/**********************************************************************************************//**
 * Gets peep minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep minimum para ippv.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPEEP_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPEEP_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets peep minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep minimum para trigger.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPEEP_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_TRIGGER;
}

/**********************************************************************************************//**
 * Gets peep minimum para risetime
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep minimum para risetime.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMinPara_RISETIME()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataPEEP_RISETIME;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataPEEP_RISETIME;
}

/**********************************************************************************************//**
 * Gets bpm minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpm minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetBPMMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMinRange_NEONATAL.m_iParaDataBPM_IPPV;
	}
	else
	{
		return m_dataMinRange_PEDIATRIC.m_iParaDataBPM_IPPV;
	}
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMinRange_NEONATAL.m_iParaDataBPM_TRIGGER;
	}
	else
	{
		return m_dataMinRange_PEDIATRIC.m_iParaDataBPM_TRIGGER;
	}
#endif
}

/**********************************************************************************************//**
 * Gets bpm minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpm minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetBPMMinPara_TRIGGER()
{
	bool bSIMVmode=false;
	if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
		bSIMVmode=true;

	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(bSIMVmode)
			return m_dataMinRange_NEONATAL.m_iParaDataBPM_SIMV;
		else
			return m_dataMinRange_NEONATAL.m_iParaDataBPM_TRIGGER;
	}
	else
	{
		if(bSIMVmode)
			return m_dataMinRange_PEDIATRIC.m_iParaDataBPM_SIMV;
		else
			return m_dataMinRange_PEDIATRIC.m_iParaDataBPM_TRIGGER;
	}
}

/**********************************************************************************************//**
 * Gets bpmnmode minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The bpmnmode minimum para.
 **************************************************************************************************/

WORD CParaData::GetBPMNMODEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataBPM_NMODE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataBPM_NMODE;
}

/**********************************************************************************************//**
 * Gets i time minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetITimeMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataITime_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataITime_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataITime_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataITime_TRIGGER;
#endif
	
}

/**********************************************************************************************//**
 * Gets i time minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetITimeMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataITime_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataITime_TRIGGER;
}

/**********************************************************************************************//**
 * Gets i time nmode minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time nmode minimum para.
 **************************************************************************************************/

WORD CParaData::GetITimeNMODEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataITime_NMODE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataITime_NMODE;
}

/**********************************************************************************************//**
 * Gets etime minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etime minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetETIMEMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMinRange_NEONATAL.m_iParaDataETime_IPPV;
	}
	else
	{
		return m_dataMinRange_PEDIATRIC.m_iParaDataETime_IPPV;
	}
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		return m_dataMinRange_NEONATAL.m_iParaDataETime_TRIGGER;
	}
	else
	{
		return m_dataMinRange_PEDIATRIC.m_iParaDataETime_TRIGGER;
	}
#endif
}

/**********************************************************************************************//**
 * Gets etime minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etime minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetETIMEMinPara_TRIGGER()
{
	bool bSIMVmode=false;
	if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
		bSIMVmode=true;

	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(bSIMVmode)
			return m_dataMinRange_NEONATAL.m_iParaDataETime_SIMV;
		else
			return m_dataMinRange_NEONATAL.m_iParaDataETime_TRIGGER;
	}
	else
	{
		if(bSIMVmode)
			return m_dataMinRange_PEDIATRIC.m_iParaDataETime_SIMV;
		else
			return m_dataMinRange_PEDIATRIC.m_iParaDataETime_TRIGGER;
	}
}

/**********************************************************************************************//**
 * Gets etimenmode minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The etimenmode minimum para.
 **************************************************************************************************/

WORD CParaData::GetETIMENMODEMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataETime_NMODE;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataETime_NMODE;
}

/**********************************************************************************************//**
 * Gets i flow minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetIFlowMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataIFlow_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataIFlow_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets i flow minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetIFlowMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataIFlow_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataIFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Gets hf flow minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf flow minimum para.
 **************************************************************************************************/

WORD CParaData::GetHFFlowMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataHFFlow;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataHFFlow;
}

/**********************************************************************************************//**
 * Gets risetime minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The risetime minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetRisetimeMinPara_IPPV()
{
#ifdef BACKUP_VENTILATION
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataRisetime_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_IPPV;
#else
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataRisetime_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER;
#endif
}

/**********************************************************************************************//**
 * Gets risetime minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The risetime minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetRisetimeMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataRisetime_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataRisetime_TRIGGER;
}

/**********************************************************************************************//**
 * Gets v limit minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v limit minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetVLimitMinPara_IPPV()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataVLimit_IPPV;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataVLimit_IPPV;
}

/**********************************************************************************************//**
 * Gets v limit minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v limit minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetVLimitMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataVLimit_TRIGGER;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataVLimit_TRIGGER;
}

/**********************************************************************************************//**
 * Gets v garant minimum para ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v garant minimum para ippv.
 **************************************************************************************************/

WORD CParaData::GetVGarantMinPara_IPPV()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		/*if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
			return 50;
		else*/
			return m_dataMinRange_NEONATAL.m_iParaDataVGarant_IPPV;
	}
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_IPPV;
}

/**********************************************************************************************//**
 * Gets v garant minimum para trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The v garant minimum para trigger.
 **************************************************************************************************/

WORD CParaData::GetVGarantMinPara_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		/*if(getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_LINEAR)
			return 50;
		else*/
			return m_dataMinRange_NEONATAL.m_iParaDataVGarant_TRIGGER;
	}
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_TRIGGER;
}

/**********************************************************************************************//**
 * Gets hfv garant minimum para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfv garant minimum para.
 **************************************************************************************************/

WORD CParaData::GetHFVGarantMinPara()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataMinRange_NEONATAL.m_iParaDataVGarant_HFO;
	else
		return m_dataMinRange_PEDIATRIC.m_iParaDataVGarant_HFO;
}

/**********************************************************************************************//**
 * Gets p manual maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual maximum key.
 **************************************************************************************************/

SHORT CParaData::GetPManualMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataPManual_HFO;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataPManual_HFO;
}

/**********************************************************************************************//**
 * Gets p manual nmode maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual nmode maximum key.
 **************************************************************************************************/

SHORT CParaData::GetPManualNMODEMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataPManual_NMODE;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataPManual_NMODE;
}

/**********************************************************************************************//**
 * Gets p manual minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual minimum key.
 **************************************************************************************************/

SHORT CParaData::GetPManualMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataPManual_HFO;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataPManual_HFO;
}

/**********************************************************************************************//**
 * Gets p manual nmode minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p manual nmode minimum key.
 **************************************************************************************************/

SHORT CParaData::GetPManualNMODEMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataPManual_NMODE;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataPManual_NMODE;
}

/**********************************************************************************************//**
 * Gets therapie flow maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The therapie flow maximum key.
 **************************************************************************************************/

WORD CParaData::GetTherapieFlowMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataTherapieFlow;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataTherapieFlow;
}

/**********************************************************************************************//**
 * Gets therapie flow minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The therapie flow minimum key.
 **************************************************************************************************/

SHORT CParaData::GetTherapieFlowMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataTherapieFlow;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataTherapieFlow;
}

/**********************************************************************************************//**
 * Gets cpap maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpap maximum key.
 **************************************************************************************************/

SHORT CParaData::GetCPAPMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataCPAP;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataCPAP;
}

/**********************************************************************************************//**
 * Gets cpapnmode maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpapnmode maximum key.
 **************************************************************************************************/

SHORT CParaData::GetCPAPNMODEMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataCPAP_NMODE;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataCPAP_NMODE;
}

/**********************************************************************************************//**
 * Gets p insp maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p insp maximum key.
 **************************************************************************************************/

SHORT CParaData::GetPInspMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataPInsp_TRIGGER;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataPInsp_TRIGGER;
}

/**********************************************************************************************//**
 * Gets i flow maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i flow maximum key.
 **************************************************************************************************/

WORD CParaData::GetIFlowMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataIFlow_TRIGGER;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataIFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Gets e flow maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The e flow maximum key.
 **************************************************************************************************/

WORD CParaData::GetEFlowMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataEFlow_TRIGGER;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataEFlow_TRIGGER;
}

/**********************************************************************************************//**
 * Gets vlimit maximum key ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The vlimit maximum key ippv.
 **************************************************************************************************/

WORD CParaData::GetVlimitMaxKey_IPPV()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataVLimit_IPPV;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataVLimit_IPPV;
}

/**********************************************************************************************//**
 * Gets vlimit maximum key trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The vlimit maximum key trigger.
 **************************************************************************************************/

WORD CParaData::GetVlimitMaxKey_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataVLimit_TRIGGER;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataVLimit_TRIGGER;
}

/**********************************************************************************************//**
 * Gets vgarant maximum key ippv
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The vgarant maximum key ippv.
 **************************************************************************************************/

WORD CParaData::GetVgarantMaxKey_IPPV()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataVGarant_IPPV;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataVGarant_IPPV;
}

/**********************************************************************************************//**
 * Gets vgarant maximum key trigger
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The vgarant maximum key trigger.
 **************************************************************************************************/

WORD CParaData::GetVgarantMaxKey_TRIGGER()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataVGarant_TRIGGER;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataVGarant_TRIGGER;
}

/**********************************************************************************************//**
 * Gets hf vgarant maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hf vgarant maximum key.
 **************************************************************************************************/

WORD CParaData::GetHFVgarantMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataVGarant_HFO;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataVGarant_HFO;
}

/**********************************************************************************************//**
 * Gets ppsv maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The ppsv maximum key.
 **************************************************************************************************/

SHORT CParaData::GetPpsvMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataPpsv;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataPpsv;
}

/**********************************************************************************************//**
 * Gets peep maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep maximum key.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataPEEP_TRIGGER;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataPEEP_TRIGGER;
}

/**********************************************************************************************//**
 * Gets hfp mean maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean maximum key.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataHFPmean;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataHFPmean;
}

/**********************************************************************************************//**
 * Gets i time record maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i time record maximum key.
 **************************************************************************************************/

WORD CParaData::GetITimeRecMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataITIME_REC;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataITIME_REC;
}

/**********************************************************************************************//**
 * Gets frequency record maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The frequency record maximum key.
 **************************************************************************************************/

WORD CParaData::GetFreqRecMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataFREQ_REC;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataFREQ_REC;
}

/**********************************************************************************************//**
 * Gets hfp mean record maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The hfp mean record maximum key.
 **************************************************************************************************/

WORD CParaData::GetHFPMeanRecMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataPMEAN_REC;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataPMEAN_REC;
}

//WORD CParaData::GetHFampMaxKey()
//{
//	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
//	{
//		return m_dataKeyMax_NEONATAL.m_iParaDataHFAMPL;
//	}
//	else
//	{
//		return m_dataKeyMax_PEDIATRIC.m_iParaDataHFAMPL;
//	}
//}

/**********************************************************************************************//**
 * Gets h ffreq maximum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The h ffreq maximum key.
 **************************************************************************************************/

BYTE CParaData::GetHFfreqMaxKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMax_NEONATAL.m_iParaDataHFFreq;
	else
		return m_dataKeyMax_PEDIATRIC.m_iParaDataHFFreq;
}

/**********************************************************************************************//**
 * Gets cpap minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpap minimum key.
 **************************************************************************************************/

SHORT CParaData::GetCPAPMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataCPAP;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataCPAP;
}

/**********************************************************************************************//**
 * Gets cpapnmode minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The cpapnmode minimum key.
 **************************************************************************************************/

SHORT CParaData::GetCPAPNMODEMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataCPAP_NMODE;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataCPAP_NMODE;
}

/**********************************************************************************************//**
 * Gets p insp minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The p insp minimum key.
 **************************************************************************************************/

SHORT CParaData::GetPInspMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataPInsp_TRIGGER;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataPInsp_TRIGGER;
}

/**********************************************************************************************//**
 * Gets ppsv minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The ppsv minimum key.
 **************************************************************************************************/

SHORT CParaData::GetPpsvMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataPpsv;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataPpsv;
}

/**********************************************************************************************//**
 * Gets peep minimum key
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The peep minimum key.
 **************************************************************************************************/

SHORT CParaData::GetPEEPMinKey()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		return m_dataKeyMin_NEONATAL.m_iParaDataPEEP_TRIGGER;
	else
		return m_dataKeyMin_PEDIATRIC.m_iParaDataPEEP_TRIGGER;
}