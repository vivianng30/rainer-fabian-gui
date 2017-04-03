//=============================================================================
/** 
* \file InterfaceAcuLink.cpp
* \brief Functions for acuLink interface.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
*		Kernel NetDCU11 V1.27  Oct 19 2012 necessary -> serial DLL!!!
**/
//=============================================================================
#include "StdAfx.h"
#include "MVModel.h"
#include "TlsFile.h"
#include "TlsStream.h"

#include "InterfaceAcuLink.h"
#include "InterfaceAcuLink_EXTERNAL.h"
#include "InterfaceAcuLink_SERIAL.h"

CInterfaceAcuLink* CInterfaceAcuLink::theAcuLinkInterface=0;

HINSTANCE CInterfaceAcuLink::_hAcuLink=NULL;
int	CInterfaceAcuLink::fiLinkType=ACL_LT_NONE;



CInterfaceAcuLink::CInterfaceAcuLink(void)
{
	m_bShowAcuLinkError=true;

	fiLinkType = ACL_LT_NONE;

	m_iVersion=0;
}

CInterfaceAcuLink::~CInterfaceAcuLink(void)
{

}

// **************************************************************************
// 
// **************************************************************************
CInterfaceAcuLink* CInterfaceAcuLink::getInstance(BYTE iLinkType)
{
	if(theAcuLinkInterface == 0)
	{
		switch(iLinkType)
		{
		case ACL_RS232_DATA:
		case ACL_RS232_WAVES:
			{
				theAcuLinkInterface = new CInterfaceAcuLink_SERIAL();
			}
			break;
		/*case ACL_RJ45_DATA:
		case ACL_RJ45_WAVES:
			{
				theAcuLinkInterface = new CInterfaceAcuLinkRJ45();
			}
			break;*/
		default:
			{
				theAcuLinkInterface = new CInterfaceAcuLink_EXTERNAL();
			}
			break;
		}

		fiLinkType=iLinkType;
	}
	return theAcuLinkInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceAcuLink::destroyInstance()
{
	if(theAcuLinkInterface != NULL)
	{
		delete theAcuLinkInterface;
		theAcuLinkInterface = NULL;
	}
}



// **************************************************************************
// 
// **************************************************************************
void CInterfaceAcuLink::fillMeasurementData()
{
	bool bCO2value=false;
	bool bCO2module=false;
	bool bSPO2module=false;
	
	if(false==getModel()->isCO2inprogress())
	{
		if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE && getModel()->getETCO2()!=NULL)
		{
			bCO2module=true;
		}

		/*if(	bCO2module && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			bCO2value=true;
		}*/
	}

	if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		bSPO2module=true;
	}
	

	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	switch(eActiveVentMode)
	{
	case VM_IPPV:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_PEEP));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_C20C));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVE));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_TVI));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TRIGGER,ALINK_NOTVALID);		//SIPPV / SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_RESISTANCE)/10))/10);
			
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}

			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_SIPPV:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TRIGGER,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER));		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI));			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO

			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV 
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE)/10))/10);
			
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}

			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_PSV:
		{
			setMeasurementData(ALINK_MSMNT_TVE_RESP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_RESP));	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT)); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TRIGGER,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER));		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI));			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_T_INSP_PSV));	//SIMV+PSV /PSV 
			
			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			int iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
			int iTVEpat=getModel()->getDATAHANDLER()->getMessureDataTVEpat();
			int iRSBI=0;
			if(iTVEpat!=0)
			{
				iRSBI=(int)CTlsFloat::Round((double)(iBPM/((double)iTVEpat/10))*10, 0);
			}
			setMeasurementData(ALINK_MSMNT_RSBI,iRSBI);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE)/10))/10);
			
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}

			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_SIMV:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP));	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_RESP));	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT)); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TRIGGER,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER));		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI));			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO

			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV  
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			int iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
			int iTVEpat=getModel()->getDATAHANDLER()->getMessureDataTVEpat();
			int iRSBI=0;
			if(iTVEpat!=0)
			{
				iRSBI=(int)CTlsFloat::Round((double)(iBPM/((double)iTVEpat/10))*10, 0);
			}
			setMeasurementData(ALINK_MSMNT_RSBI,iRSBI);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE)/10))/10);
			
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_SIMVPSV:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP));	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_RESP));	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT)); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TRIGGER,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER));		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI));			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_T_INSP_PSV));	//SIMV+PSV /PSV  
			

			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			int iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
			int iTVEpat=getModel()->getDATAHANDLER()->getMessureDataTVEpat();
			int iRSBI=0;
			if(iTVEpat!=0)
			{
				iRSBI=(int)CTlsFloat::Round((double)(iBPM/((double)iTVEpat/10))*10, 0);
			}
			setMeasurementData(ALINK_MSMNT_RSBI,iRSBI);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE)/10))/10);
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_CPAP:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE));			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DEMAND_FLOW));
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI));			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO

			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_TRIGGER,ALINK_NOTVALID);		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV 
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataTVE()*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE)/10))/10);
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_DUOPAP:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_PEEP));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_IFLOW,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_IFLOW));
			setMeasurementData(ALINK_MSMNT_EFLOW,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_EFLOW));
			setMeasurementData(ALINK_MSMNT_TRIGGER,ALINK_NOTVALID);		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,ALINK_NOTVALID);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,ALINK_NOTVALID);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,ALINK_NOTVALID);			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,ALINK_NOTVALID);		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVI,ALINK_NOTVALID);			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV 
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,ALINK_NOTVALID);
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_NCPAP:
		{
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_IFLOW,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_IFLOW));
			setMeasurementData(ALINK_MSMNT_EFLOW,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_EFLOW));
			setMeasurementData(ALINK_MSMNT_P_PEAK,ALINK_NOTVALID);		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,ALINK_NOTVALID);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,ALINK_NOTVALID);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,ALINK_NOTVALID);			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,ALINK_NOTVALID);		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_TRIGGER,ALINK_NOTVALID);		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV 
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,ALINK_NOTVALID);
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_THERAPIE:
		{
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO

			setMeasurementData(ALINK_MSMNT_FREQ,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_P_PEAK,ALINK_NOTVALID);		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_PEEP,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,ALINK_NOTVALID);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,ALINK_NOTVALID);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVE,ALINK_NOTVALID);			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_LEAK,ALINK_NOTVALID);		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVI,ALINK_NOTVALID);			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_HFO_AMP,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,ALINK_NOTVALID);		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,ALINK_NOTVALID);	//HFO
			setMeasurementData(ALINK_MSMNT_TRIGGER,ALINK_NOTVALID);		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV 
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,ALINK_NOTVALID);
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	case VM_HFO:
		{
			setMeasurementData(ALINK_MSMNT_P_PEAK,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK));		//NUMT_PPEAK	//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_P_MEAN,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_P_MEAN));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_DYNCOMPL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/10);	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_C20C,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_RESISTANCE,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE));	//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_MIN_VOL,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL));		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_LEAK,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK));		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_TVI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI));			//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
			setMeasurementData(ALINK_MSMNT_HFO_AMP,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_AMP));		//HFO
			setMeasurementData(ALINK_MSMNT_TVE_HFO,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_HFO));		//HFO
			setMeasurementData(ALINK_MSMNT_DCO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DCO2));		//HFO
			setMeasurementData(ALINK_MSMNT_HFO_FREQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_FREQ));	//HFO

			setMeasurementData(ALINK_MSMNT_TRIGGER,ALINK_NOTVALID);		//SIPPV /SIMV / SIMV+PSV / PSV 
			setMeasurementData(ALINK_MSMNT_FREQ,ALINK_NOTVALID);		//IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_ANTEIL_RESP,ALINK_NOTVALID);	//SIMV / SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE,ALINK_NOTVALID);			//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_TVE_RESP,ALINK_NOTVALID);	//SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_TVE_PATIENT,ALINK_NOTVALID); //SIMV/ SIMV+PSV 
			setMeasurementData(ALINK_MSMNT_PEEP,ALINK_NOTVALID);		//IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
			setMeasurementData(ALINK_MSMNT_T_INSP_PSV,ALINK_NOTVALID);	//SIMV+PSV /PSV 
			setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_EFLOW,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,ALINK_NOTVALID);

			//NEWACULINK
			int iBW=getModel()->getDATAHANDLER()->GetBodyweight();
			if(iBW!=0)
			{
				double iTVE=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_HFO)*100;
				int iTVEKG=(int)CTlsFloat::Round((double)(iTVE/iBW)*10, 0);
				setMeasurementData(ALINK_MSMNT_VTEKG,iTVEKG);
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_VTEKG,ALINK_NOTVALID);
			}
			
			setMeasurementData(ALINK_MSMNT_RSBI,ALINK_NOTVALID);
			setMeasurementData(ALINK_MSMNT_TAU,((getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL)/100)*(getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE)/10))/10);
			if(bCO2module)
			{
				if(getModel()->getETCO2()->isETCO2ValueValid())
					setMeasurementData(ALINK_MSMNT_ETCO2,getModel()->getDATAHANDLER()->getAVGMessureDataETCO2());
				else
					setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);

				setMeasurementData(ALINK_MSMNT_FREQETCO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FREQETCO2));	
				setMeasurementData(ALINK_MSMNT_FICO2,getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_FICO2));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_ETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FREQETCO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_FICO2,ALINK_NOTVALID);
			}
			if(bSPO2module)
			{
				setMeasurementData(ALINK_MSMNT_SPO2,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2));
				setMeasurementData(ALINK_MSMNT_SPO2_PI,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PI));
				setMeasurementData(ALINK_MSMNT_SPO2_PR,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_PR));
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ));
			}
			else
			{
				setMeasurementData(ALINK_MSMNT_SPO2,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PI,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_PR,ALINK_NOTVALID);
				setMeasurementData(ALINK_MSMNT_SPO2_SIQ,ALINK_NOTVALID);
			}
		}
		break;
	default:
		{
			
		}
		break;
	}
}




void CInterfaceAcuLink::clearAlarmArray()
{
	setAlarmData(ALINK_ALARM_PatAl_Apnoe,0);
	setAlarmData(ALINK_ALARM_TUBUSBLOCKED,0);
	setAlarmData(ALINK_ALARM_DISCONNECTION,0);
	setAlarmData(ALINK_ALARM_SysAl_P_IN_O2,0);
	setAlarmData(ALINK_ALARM_SysAl_P_IN_AIR,0);
	/*setAlarmData(ALINK_ALARM_SysAl_P_EXSPIRATIONTUBE,0);
	setAlarmData(ALINK_ALARM_SysAl_P_INSPIRATIONTUBE,0);*/
	setAlarmData(ALINK_ALARM_Sens_O2_SENSOR_DEFECT,0);
	setAlarmData(ALINK_ALARM_Sens_O2_NOT_CALIBRATED,0);
	setAlarmData(ALINK_ALARM_Sens_O2_SENSOR_USED,0);
	setAlarmData(ALINK_ALARM_Sens_O2_VALUE_INCORRECT,0);
	setAlarmData(ALINK_ALARM_SysFail_RELAIS_DEFECT,0);
	setAlarmData(ALINK_ALARM_SysFail_MIXER,0);
	setAlarmData(ALINK_ALARM_SysAl_TUBE_OCCLUSION,0);
	setAlarmData(ALINK_ALARM_SysFail_P_IN_MIXER,0);
	setAlarmData(ALINK_ALARM_SysFail_OVERPRESSURE,0);
	setAlarmData(ALINK_ALARM_SysFail_VOLTAGE,0);
	setAlarmData(ALINK_ALARM_SysFail_OUTOFMEMORY,0);
	setAlarmData(ALINK_ALARM_SysFail_ChecksumConPIC,0);
	setAlarmData(ALINK_ALARM_SysFail_ChecksumMonPIC,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_SPI,0);
	setAlarmData(ALINK_ALARM_SysFail_Fan,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_DIO,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_COM,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_I2C,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_CO2,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_PIF,0);
	setAlarmData(ALINK_ALARM_SysFail_IF_ACULINK,0);
	setAlarmData(ALINK_ALARM_PatAl_MVmax,0);
	setAlarmData(ALINK_ALARM_PatAl_MVmin,0);
	setAlarmData(ALINK_ALARM_PatAl_Pmax,0);
	setAlarmData(ALINK_ALARM_PatAl_PminLow,0);
	setAlarmData(ALINK_ALARM_PatAl_PminHigh,0);
	setAlarmData(ALINK_ALARM_PatAl_BPMmax,0);
	setAlarmData(ALINK_ALARM_PatAl_Leakmax,0);
	setAlarmData(ALINK_ALARM_PatAl_DCO2max,0);
	setAlarmData(ALINK_ALARM_PatAl_DCO2min,0);
	setAlarmData(ALINK_ALARM_PatAl_ETCO2max,0);
	setAlarmData(ALINK_ALARM_PatAl_ETCO2min,0);
	setAlarmData(ALINK_ALARM_Nebulizer_Disconnection,0);
	setAlarmData(ALINK_ALARM_Nebulizer_SysError,0);
	setAlarmData(ALINK_ALARM_CO2_MODULE_NOTCONNECTED,0);
	setAlarmData(ALINK_ALARM_CO2_FILTERLINE_NOTCONNECTED,0);
	setAlarmData(ALINK_ALARM_CO2_CHECKSAMPLINGLINE,0);
	setAlarmData(ALINK_ALARM_CO2_CHECKAIRWAYADAPTER,0);
	setAlarmData(ALINK_ALARM_CO2_SENSORFAULTY,0);
	setAlarmData(ALINK_ALARM_Sens_FLOW_SENSOR_DEFECT,0);
	setAlarmData(ALINK_ALARM_Sens_FLOW_SENSOR_CLEANING,0);
	setAlarmData(ALINK_ALARM_Sens_FLOW_SENSOR_NOTCONNECTED,0);
	setAlarmData(ALINK_ALARM_SysLimit_Vlimitted,0);
	setAlarmData(ALINK_ALARM_SysLimit_Vgaranty,0);
	setAlarmData(ALINK_ALARM_SysLimit_Pinsp_NotReached,0);
	setAlarmData(ALINK_ALARM_Accu_POWER,0);
	setAlarmData(ALINK_ALARM_Accu_60,0);
	setAlarmData(ALINK_ALARM_Accu_30,0);
	setAlarmData(ALINK_ALARM_Accu_15,0);
	setAlarmData(ALINK_ALARM_Accu_Empty,0);
	setAlarmData(ALINK_ALARM_Accu_Defect,0);
	setAlarmData(ALINK_ALARM_PRICO_FiO2max,0);
	setAlarmData(ALINK_ALARM_PRICO_FiO2min,0);
	setAlarmData(ALINK_ALARM_Sens_PRICO_FiO2outOfRange,0);
	setAlarmData(ALINK_ALARM_Sens_SPO2_MODULE_NOTCONNECTED,0);
	setAlarmData(ALINK_ALARM_Sens_SPO2_SENSORFAULTY,0);
	setAlarmData(ALINK_ALARM_Sens_SPO2_CHECKSENSOR,0);
	setAlarmData(ALINK_ALARM_PatAl_SPO2_SIQmin,0);
	setAlarmData(ALINK_ALARM_PatAl_SPO2max,0);
	setAlarmData(ALINK_ALARM_PatAl_SPO2min,0);
	setAlarmData(ALINK_ALARM_PatAl_SPO2_PImin,0);
	setAlarmData(ALINK_ALARM_PatAl_PIPmin,0);
	setAlarmData(ALINK_ALARM_PatAl_FICO2max,0);
	setAlarmData(ALINK_ALARM_PatAl_FICO2min,0);
	setAlarmData(ALINK_ALARM_PatAl_PulseRatemax,0);
	setAlarmData(ALINK_ALARM_PatAl_PulseRatemin,0);
	//setAlarmData(ALINK_ALARM_PatAl_PMEANmax,0);
	//setAlarmData(ALINK_ALARM_PatAl_PMEANmin,0);
	Sleep(0);

}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceAcuLink::setLinkType(int iLinkType)
{
	fiLinkType=iLinkType;
	
	return 0;
}


// **************************************************************************
// 
// **************************************************************************
UINT CInterfaceAcuLink::getAcuLinkError()
{
	//if(pGetAclError)
	//{
	//	m_iAclError=(*pGetAclError)();
	//}
	
	return 0;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceAcuLink::setNetPort(UINT uiPort)
{
	/*if(pSetNetPortFunc)
		((*pSetNetPortFunc)(uiPort));*/

	return 0;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceAcuLink::setLanguage(UINT uiLang)
{
	/*if(pSetLanguageFunc)
		((*pSetLanguageFunc)(uiLang));*/

	return 0;
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceAcuLink::setGraphData(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2, SHORT iValSPO2)
{
	//WaveRecord rWaveRecord
	////in ring buffer von DLL schreiben
	//if(pSetWaveRecord)
	//{
	//	int iRet=((*pSetWaveRecord)(iValVolume ,iValPressure ,iValFlow, iValCO2));
	//	if(iRet!=0)
	//	{
	//		if(m_bShowAcuLinkError==true)
	//		{
	//			m_bShowAcuLinkError=false;
	//			/*CStringW sz=_T("");
	//			sz.Format(_T("ERROR acuLink SetGraphData: %d"), iRet);
	//			theApp.getLog()->WriteLine(sz);*/
	//		}
	//	}
	//}
}
