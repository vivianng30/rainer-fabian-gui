// InterfaceAcuLink_EXTERNAL.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceAcuLink_EXTERNAL.h"
#include "MVModel.h"
#include "TlsFile.h"
#include "TlsStream.h"

CStringW CInterfaceAcuLink_EXTERNAL::m_szFileAcuLink=_T("");


LPSETINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pSetAcuLinkVersion=0;
LPGETINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pGetAcuLinkVersion=0;

LP_SET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pSetMeasurementData=0;
LP_GET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pGetMeasurementData=0;

LP_SET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pSetSettingData=0;
LP_GET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pGetSettingData=0;

LP_SET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pSetAlarmData=0;
LP_GET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pGetAlarmData=0;

LP_SET_ARRAY_DATA_DLLFUNC CInterfaceAcuLink_EXTERNAL::pSayHelloStr=0;

LP_SET_WAVE_RECORD_DLLFUNC CInterfaceAcuLink_EXTERNAL::pSetWaveRecord=0;

LP_SET_BOOL_DLLFUNC CInterfaceAcuLink_EXTERNAL::pSetProducerStarted=0;

LP_GETACLERROR_DLLFUNC CInterfaceAcuLink_EXTERNAL::pGetAclError=0;
LPSETINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pSetLinkTypeFunc=0;
LPSETUINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pSetNetPortFunc=0;
LPSETUINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pSetLanguageFunc=0;

LPSETUINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pSetShutdownFunc=0;
LPGETUINTDLLFUNC CInterfaceAcuLink_EXTERNAL::pCheckShutdownFunc=0;


// CInterfaceAcuLink_EXTERNAL


CInterfaceAcuLink_EXTERNAL::CInterfaceAcuLink_EXTERNAL()
{
	m_iVersion=0;
}

CInterfaceAcuLink_EXTERNAL::~CInterfaceAcuLink_EXTERNAL()
{
	if(_hAcuLink !=NULL)
	{
		::FreeLibrary(_hAcuLink);

		_hAcuLink=NULL;
	}
}



// CInterfaceAcuLink_EXTERNAL message handlers
// **************************************************************************
// 
// **************************************************************************
bool CInterfaceAcuLink_EXTERNAL::init()
{
	m_szVersionExe=_T("");
	m_szVersionDll=_T("");
	m_iVersion=0;

	CStringW szLicenseFile=_T("ML");
	szLicenseFile+=CInterface::getModel()->GetUniqueID();
	szLicenseFile+=_T(".mlic");
	m_szFileAcuLink=_T("\\FFSDISK\\")+szLicenseFile;

	bool bRes=false;


	if(		CTlsFile::Exists(_T("\\FFSDISK\\AcuLink.exe"))
		&&	CTlsFile::Exists(_T("\\FFSDISK\\AcuLink_DLL.dll"))
		&&	CTlsFile::Exists(m_szFileAcuLink))
	{
		if(_hAcuLink == NULL)
			_hAcuLink = ::LoadLibrary(_T("AcuLink_DLL.dll"));	
	}

	if(_hAcuLink !=NULL)
	{
		pSetAcuLinkVersion = (LPSETINTDLLFUNC)::GetProcAddress(_hAcuLink, L"SetAcuLinkVersion");
		pGetAcuLinkVersion = (LPGETINTDLLFUNC)::GetProcAddress(_hAcuLink, L"GetAcuLinkVersion");

		pSetShutdownFunc = (LPSETUINTDLLFUNC)::GetProcAddress(_hAcuLink, L"SetShutdown");
		pCheckShutdownFunc = (LPGETUINTDLLFUNC)::GetProcAddress(_hAcuLink, L"CheckShutdown");

		pSetMeasurementData = (LP_SET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"SetMeasurementData");
		pGetMeasurementData = (LP_GET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"GetMeasurementData");

		pSetSettingData = (LP_SET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"SetSettingData");
		pGetSettingData = (LP_GET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"GetSettingData");

		pSetAlarmData = (LP_SET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"SetAlarmData");
		pGetAlarmData = (LP_GET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"GetAlarmData");

		pSayHelloStr = (LP_SET_ARRAY_DATA_DLLFUNC)::GetProcAddress(_hAcuLink, L"SayHelloStr");

		pSetWaveRecord = (LP_SET_WAVE_RECORD_DLLFUNC)::GetProcAddress(_hAcuLink, L"SetWaveRecord");

		pSetProducerStarted= (LP_SET_BOOL_DLLFUNC)::GetProcAddress(_hAcuLink, L"SetProducerStarted");

		pGetAclError=(LP_GETACLERROR_DLLFUNC)::GetProcAddress(_hAcuLink, L"GetAclError");
		pSetLinkTypeFunc=(LPSETINTDLLFUNC)::GetProcAddress(_hAcuLink, L"SetLinkType");
		pSetNetPortFunc=(LPSETUINTDLLFUNC)::GetProcAddress(_hAcuLink, L"SetNetPort");
		pSetLanguageFunc=(LPSETUINTDLLFUNC)::GetProcAddress(_hAcuLink, L"SetLanguage");

		//PROCs überprüfen......
		//if(pGetAcuLinkError==0)
		//{
		//	//ERROR todo logfile
		//	_hAcuLink=NULL;
		//}
		if(pSetAcuLinkVersion==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pGetAcuLinkVersion==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}

		if(pSetShutdownFunc==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pCheckShutdownFunc==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}

		if(pSetMeasurementData==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pGetMeasurementData==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetSettingData==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pGetSettingData==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetAlarmData==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pGetAlarmData==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSayHelloStr==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetWaveRecord==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetProducerStarted==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}

		if(pGetAclError==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetLinkTypeFunc==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetNetPortFunc==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}
		if(pSetLanguageFunc==0)
		{
			//ERROR todo logfile
			::FreeLibrary(_hAcuLink);

			_hAcuLink=NULL;
		}

		if(_hAcuLink==NULL)
		{
			AfxMessageBox( _T("_hAcuLink==NULL") );

			pSetAcuLinkVersion=0;
			pGetAcuLinkVersion=0;

			pSetShutdownFunc =0;
			pCheckShutdownFunc =0;

			pSetMeasurementData=0;
			pGetMeasurementData=0;

			pSetSettingData=0;
			pGetSettingData=0;

			pSetAlarmData=0;
			pGetAlarmData=0;

			pSayHelloStr=0;

			pSetWaveRecord=0;

			pSetProducerStarted=0;
		}

	}

	if(_hAcuLink !=NULL)
	{
		if(pGetAcuLinkVersion)
		{
			m_iVersion=(*pGetAcuLinkVersion)();
		}
		int iExeHauptVersion = (m_iVersion & (UINT)0xFF000000) >> 24;
		int iExeNebenVersion = (m_iVersion & (UINT)0x00FF0000) >> 16;
		int iDllHauptVersion = (m_iVersion & (UINT)0x0000FF00) >> 8;
		int iDllNebenVersion = (m_iVersion & (UINT)0x000000FF);

		m_szVersionExe.Format(_T("%d.%d"),iExeHauptVersion, iExeNebenVersion);
		m_szVersionDll.Format(_T("%d.%d"),iDllHauptVersion, iDllNebenVersion);

		bRes=true;

		CStringW sz=_T("");
		sz.Format(_T("***Version acuLink: %s %s"), m_szVersionExe, m_szVersionDll);
		theApp.getLog()->WriteLine(sz);
	}
	else
	{
		//bRes=false;
		m_iVersion=999;
	}

	clearAlarmArray();

	setParaData(ALINK_SETT_VERSION_ACULINK,m_iVersion);

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceAcuLink_EXTERNAL::deinit()
{

	return true;
}

//HINSTANCE CInterfaceAcuLink_EXTERNAL::isAcuLink()
//{
//	return _hAcuLink;
//}

void CInterfaceAcuLink_EXTERNAL::setShutdown(UINT uiVal)
{
	if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII)
	{
		//do nothing here
	}
	else if(pSetShutdownFunc)
		((*pSetShutdownFunc)(uiVal));
}

void CInterfaceAcuLink_EXTERNAL::setProducerStarted(bool bState)
{
	if(pSetProducerStarted)
		((*pSetProducerStarted)(bState));
}

void CInterfaceAcuLink_EXTERNAL::setGraphData(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2, SHORT iValSPO2)
{
	//WaveRecord rWaveRecord
	////in ring buffer von DLL schreiben
	if(pSetWaveRecord)
	{
		int iRet=((*pSetWaveRecord)(iValVolume ,iValPressure ,iValFlow, iValCO2, iValSPO2));
		if(iRet!=0)
		{
			if(m_bShowAcuLinkError==true)
			{
				m_bShowAcuLinkError=false;
				/*CStringW sz=_T("");
				sz.Format(_T("ERROR acuLink SetGraphData: %d"), iRet);
				theApp.getLog()->WriteLine(sz);*/
			}
		}
	}
}

int CInterfaceAcuLink_EXTERNAL::setLanguage(UINT uiLang)
{
	if(pSetLanguageFunc)
		((*pSetLanguageFunc)(uiLang));

	return 0;
}

int CInterfaceAcuLink_EXTERNAL::setNetPort(UINT uiPort)
{
	if(pSetNetPortFunc)
		((*pSetNetPortFunc)(uiPort));

	return 0;
}

int CInterfaceAcuLink_EXTERNAL::setLinkType(int iLinkType)
{
	if(pSetLinkTypeFunc)
		((*pSetLinkTypeFunc)(iLinkType));
	fiLinkType=iLinkType;
	return 0;
}


int CInterfaceAcuLink_EXTERNAL::setMeasurementData(int idx, int iValue)
{
	//DEBUGMSG(TRUE, (TEXT("%d|%d\r\n"),idx, iValue));
	if(pSetMeasurementData)
		((*pSetMeasurementData)(idx, iValue));

	return 0;
}

void CInterfaceAcuLink_EXTERNAL::setParaData(UINT index, int val)
{
	/*if(index==ALINK_SETT_FREQ)
	{
		DEBUGMSG(TRUE, (TEXT("setParaData ALINK_SETT_FREQ %d\r\n"),val));
	}*/
	if(pSetSettingData)
		((*pSetSettingData)(index, val));
	Sleep(0);
}

void CInterfaceAcuLink_EXTERNAL::setAlarmData(UINT index, int val)
{
	if(pSetAlarmData)
		((*pSetAlarmData)(index, val));
	Sleep(0);
}
CString CInterfaceAcuLink_EXTERNAL::getExeVersion()
{
	return m_szVersionExe;
}
CString CInterfaceAcuLink_EXTERNAL::getDllVersion()
{
	return m_szVersionDll;
}
UINT CInterfaceAcuLink_EXTERNAL::getAcuLinkError()
{
	//if(pGetAclError)
	//{
	//	m_iAclError=(*pGetAclError)();
	//}
	return 0;
}
int CInterfaceAcuLink_EXTERNAL::sendMeasurementData()
{
	fillMeasurementData();

	return 1;
}