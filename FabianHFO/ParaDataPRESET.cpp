// ParaDataPRESET.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaDataPRESET.h"
#include "MVModel.h"

CParaDataPRESET* CParaDataPRESET::thePRESETData=0;

CParaDataPRESET::CParaDataPRESET(void)
{

}

CParaDataPRESET::~CParaDataPRESET(void)
{
}


// **************************************************************************
// 
// **************************************************************************
CParaDataPRESET* CParaDataPRESET::GetInstance()
{
	if(thePRESETData == 0)
	{
		thePRESETData = new CParaDataPRESET;
		//theSIPPVData->Init();
	}
	return thePRESETData;
}

// **************************************************************************
// 
// **************************************************************************
void CParaDataPRESET::DestroyInstance()
{
	delete thePRESETData;
	thePRESETData = NULL;
}


void CParaDataPRESET::Init()
{
	
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
	m_data.m_iParaDataEFlow_TRIGGER=0;
	m_data.m_iParaDataEFlow_IPPV=0;
	m_data.m_iParaDataTherapieFlow=0;
	m_data.m_iParaDataFlowMin=0;
	m_data.m_iParaDataPEEP_TRIGGER=0;
	m_data.m_iParaDataPEEP_IPPV=0;
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
	m_data.m_iParaDataVLimit_IPPV=0;
	m_data.m_iParaDataVLimit_TRIGGER=0;
	m_data.m_bParaDataVLimitOn_IPPV=false;
	m_data.m_bParaDataVLimitOn_TRIGGER=false;
	m_data.m_iParaDataVGarant_IPPV=0;
	m_data.m_iParaDataVGarant_TRIGGER=0;
	m_data.m_bParaDataVGarantOn_IPPV=false;
	m_data.m_bParaDataVGarantOn_TRIGGER=false;
	m_data.m_iParaDataTriggerCONV=0;
	m_data.m_iParaDataTriggerNMODE=0;
	m_data.m_iParaDataBackup=0;
	m_data.m_iParaDataHFAMPL=0;
	m_data.m_iParaDataHFAMPLmax=0;
	m_data.m_iParaDataHFFreq=0;
	m_data.m_iParaDataHFPmean=0;
	m_data.m_iParaDataVGarant_HFO=0;
	m_data.m_bParaDataVGarantOn_HFO=false;
	m_data.m_iParaDataIERatio=RIE_1_1;
	m_data.m_iParaDataITIME_REC=0;
	m_data.m_iParaDataFREQ_REC=0;
	m_data.m_iParaDataPMEAN_REC=0;

	getModel()->getDATAHANDLER()->PARADATA()->GetParaData(&m_data);
}

