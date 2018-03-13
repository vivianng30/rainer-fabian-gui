// InterfaceAcuLink_ETHERNET.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceAcuLink_ETHERNET.h"
#include "MVModel.h"
#include "TlsFile.h"
#include "TlsStream.h"



// CInterfaceAcuLink_EXTERNAL


CInterfaceAcuLink_ETHERNET::CInterfaceAcuLink_ETHERNET()
{
	//            <     EXE - Version  ><DLL - Version>       
	m_iVersion = (1 << 24) | (0 << 16) | (0 << 8) | 0;
}

CInterfaceAcuLink_ETHERNET::~CInterfaceAcuLink_ETHERNET()
{
	
}



// CInterfaceAcuLink_EXTERNAL message handlers
bool CInterfaceAcuLink_ETHERNET::init()
{
	bool bRes=false;

	//if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_RS232_ASCII)
	//{
	//	// initialisiere CRC Tabelle
	//	initCRC_Table();

	//	if (!m_comPort.Open(2,38400))
	//	{
	//		//theApp.getLog()->WriteLine(_T("#HFO:0009"));
	//		return false;
	//	}
	//}
	//else if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_RS232_WAVE)
	//{
	//	// initialisiere CRC Tabelle
	//	initCRC_Table();

	//	if (!m_comPort.Open(2,115200))
	//	{
	//		//theApp.getLog()->WriteLine(_T("#HFO:0009"));
	//		return false;
	//	}
	//}

	//clearAlarmArray();

	//setParaData(ALINK_SETT_VERSION_ACULINK,m_iReturnvalue);

	return bRes;
}

bool CInterfaceAcuLink_ETHERNET::deinit()
{

	return true;
}



int CInterfaceAcuLink_ETHERNET::setMeasurementData(int idx, int iValue)
{
	

	return 0;
}

void CInterfaceAcuLink_ETHERNET::setParaData(UINT index, int val)
{
	
}

void CInterfaceAcuLink_ETHERNET::setAlarmData(UINT index, int val)
{
	
}
int CInterfaceAcuLink_ETHERNET::sendAllMeasurementData()
{
	fillMeasurementData();

	return 1;
}
