
#include "stdafx.h"
#include "TlsDevice.h"
//#include "TlsDeviceDebug.h"
//#include "TlsRegistry.h"

#include <aygshell.h>
#include <winioctl.h> 
#include "uniqueid.h"



//extern "C" __declspec(dllimport) 
//BOOL KernelIoControl(
//					 DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, 
//					 LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned
//					 );

#define IOCTL_HAL_GET_DEVICEID CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)



PIP_ADAPTER_INFO CTlsDevice::pinfo=NULL;
unsigned long CTlsDevice::len=0;
CStringW	CTlsDevice::m_macaddress=_T("");
CStringW	CTlsDevice::m_uniqueid=_T("");
CStringW	CTlsDevice::m_description=_T("");
CStringW	CTlsDevice::m_type=_T("");
CStringW	CTlsDevice::m_subnet=_T("");
CStringW	CTlsDevice::m_IpAddress=_T("");
CStringW	CTlsDevice::m_gateway=_T("");
CStringW	CTlsDevice::m_PrimaryWinsServer=_T("");
CStringW	CTlsDevice::m_dhcp=_T("");


//=============================================================================
CTlsDevice::CTlsDevice()
{
	/*pinfo=NULL;
	len=0;
	m_macaddress=_T("");
	m_description=_T("");
	m_type=_T("");
	m_subnet=_T("");
	m_IpAddress=_T("");
	m_gateway=_T("");
	m_PrimaryWinsServer=_T("");
	m_dhcp=_T("");*/
}

//=============================================================================
CTlsDevice::~CTlsDevice()
{

}

//#define IOCTL_HAL_REBOOT CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS) 
//
//#define IOCTL_HAL_GET_DEVICEID CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS) 
//BOOL KernelIoControl( DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned); 

// Get unique unit ID information from Symbol


CStringW CTlsDevice::GetSerialNumberFromKernelIoControl()
{
	DWORD dwOutBytes;
	const int nBuffSize = 4096;
	byte arrOutBuff[nBuffSize];

	BOOL bRes = ::KernelIoControl(IOCTL_HAL_GET_DEVICEID, 
		0, 0, arrOutBuff, nBuffSize, &dwOutBytes);

	if (bRes) {
		CStringW strDeviceInfo;
		for (unsigned int i = 0; i<dwOutBytes; i++) {
			CStringW strNextChar;
			strNextChar.Format(TEXT("%02X"), arrOutBuff[i]);
			strDeviceInfo += strNextChar;
		}
		CStringW strDeviceId = 
			strDeviceInfo.Mid(40,2) + 
			strDeviceInfo.Mid(45,9) + 
			strDeviceInfo.Mid(70,6);

		return strDeviceId;
	} else {
		return _T("");
	}
}

CStringW	CTlsDevice::GetDeviceID()
{
	CStringW sPlatform=_T(""); 
	CStringW sDevice=_T(""); 

	DWORD dwOutBytes; 
	const int nBuffSize = 4096; 
	byte arrOutBuff[nBuffSize]; 

	BOOL bRes = ::KernelIoControl(IOCTL_HAL_GET_DEVICEID, 0, 0, arrOutBuff, nBuffSize, &dwOutBytes); 
	DEVICE_ID* devid = (DEVICE_ID*)&arrOutBuff;

	if (bRes) 
	{ 
		for(unsigned int i1 = 0; i1 < devid->dwPlatformIDBytes; i1++) 
		{ 
			CStringW sNextChar; 
			sNextChar.Format(TEXT("%02X"), arrOutBuff[devid->dwPlatformIDOffset + i1]);
			sPlatform += sNextChar; 
		}

		for(unsigned int i2 = 0; i2 < devid->dwPresetIDBytes; i2++) 
		{ 
			CStringW sNextChar; 
			sNextChar.Format(TEXT("%02X"), arrOutBuff[devid->dwPresetIDOffset + i2]);
			sDevice += sNextChar;
		} 
	} 

	CStringW sUnitID = sPlatform + sDevice;
	return sUnitID;
	//if(sPlatform == "0050BF3F5173")
	/*{

	}*/
	

	//CStringW sUnitID;
	//if(sPlatform == "0050BF3F5173")
	//{
	//	// Platform is a Symbol PPT2800
	//	// Use Symbol-API for Device-ID
 //       HMODULE hRcmAPI = LoadLibrary(L"RcmAPI32.dll");
 //       if(!hRcmAPI)
 //       {
	//		sUnitID = sPlatform + sDevice;
 //           // TlsMessageBox("Failed to load RcmAPI32.dll", MB_OK);
 //       }
 //       else
 //       {
 //           fctUniqueID* func = (fctUniqueID*)GetProcAddress(hRcmAPI, L"RCM_GetUniqueUnitId");

 //           if(func == NULL)
 //           {
 //               TlsMessageBox("Fail to get RCM_GetUniqueUnitId", MB_OK);
 //           }
 //           else
 //           {
 //               UNITID UnitId;
 //               if (E_RCM_SUCCESS == func(&UnitId)) 
 //               {
 //                   bool bFound = false;
 //                   for (size_t n=0; n < 8; n++) 
 //                   {
 //                       if (!bFound && (UnitId[n] != 0)) 
 //                       {
 //                           bFound = true;
 //                       }
 //                       if (bFound) 
 //                       {
 //                           CStringW str;
 //                           str.Format(TEXT("%02X"), UnitId[n]);
 //                           sUnitID += str;
 //                       }
 //                   }
 //               }
 //           }

 //       }
	//}
	//else if(sPlatform == "0050BF7A60E2")
	//{
	//	// Platform is a Symbol PPT8800
	//	HMODULE hRcmAPI = LoadLibrary(L"RcmAPI32.dll");
 //       if(!hRcmAPI)
 //       {
	//		sUnitID = "";
 //           // TlsMessageBox("Failed to load RcmAPI32.dll", MB_OK);
 //       }
 //       else
 //       {
 //           fctUniqueID* func = (fctUniqueID*)GetProcAddress(hRcmAPI, L"RCM_GetUniqueUnitId");

 //           if(func == NULL)
 //           {
 //               sUnitID = "";
 //           }
 //           else
 //           {
 //               UNITID UnitId;
 //               if (E_RCM_SUCCESS == func(&UnitId)) 
 //               {
 //                   bool bFound = false;
 //                   for (size_t n=0; n < 8; n++) 
 //                   {
 //                       if (!bFound && (UnitId[n] != 0)) 
 //                       {
 //                           bFound = true;
 //                       }
 //                       if (bFound) 
 //                       {
 //                           CStringW str;
 //                           str.Format(TEXT("%02X"), UnitId[n]);
 //                           sUnitID += str;
 //                       }
 //                   }
 //               }
 //           }

 //       }

	//	// Use Buggy code from Internet...
	//	if(sUnitID == "")
	//	{
	//		CStringW strDeviceInfo; 
	//		for (unsigned int i = 0; i<dwOutBytes; i++) 
	//		{ 
	//			CStringW strNextChar; 
	//			strNextChar.Format(TEXT("%02X"), arrOutBuff[i]); 
	//			strDeviceInfo += strNextChar; 
	//		}
	//		//TlsMessageBox("PlattformPPT8800", strDeviceInfo, MB_OK);

	//		sUnitID = strDeviceInfo.Mid(40,2) + strDeviceInfo.Mid(45,9) + strDeviceInfo.Mid(70,6);
	//	}
	//	//TlsMessageBox("Plattform", "PPT8800: " + sPlatform, MB_OK);
	//	//TlsMessageBox("sDevice", sDevice, MB_OK);
	//	// Platform is a Symbol PPT8800
	//	
	//	/*CStringW strDeviceInfo; 
	//	for (unsigned int i = 0; i<dwOutBytes; i++) 
	//	{ 
	//		CStringW strNextChar; 
	//		strNextChar.Format(TEXT("%02X"), arrOutBuff[i]); 
	//		strDeviceInfo += strNextChar; 
	//	}
	//	//TlsMessageBox("PlattformPPT8800", strDeviceInfo, MB_OK);

	//	sUnitID = strDeviceInfo.Mid(40,2) + strDeviceInfo.Mid(45,9) + strDeviceInfo.Mid(70,6); */
	//	//TlsMessageBox("sUnitID1", sUnitID, MB_OK);
	//	
	//}
	//else if(sPlatform == "0040056A0E9D" || // Intermac
	//	sPlatform == "0050BF7A6749" // yakumo
	//	)
	//{

	//	sUnitID = sPlatform + sDevice;
	//}
	//else if(sPlatform == "0050BF1977E0" )	//iPaq
	//{
	//	sUnitID = sPlatform + sDevice;
	//}
	//else if(sPlatform == "00")			// WM5 Emulator
	//{
	//	sUnitID = sDevice;
	//}
	//else
	//{
	//	TlsMessageBox("Fehler", "Unbekannte Plattform: " + sPlatform, MB_OK);
	//}

	
	//return _T("");
}

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))



int CTlsDevice::GetAdapterInfo()
{
	m_macaddress=_T("");
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		return 1;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL)
		{
			return 1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			//TRACE1("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
			//TRACE1("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
			//TRACE1("\tAdapter Desc: \t%s\n", pAdapter->Description);
			//TRACE0("\tAdapter Addr: \t");
			//CStringW szTemp=_T("");
			//for (i = 0; i < pAdapter->AddressLength; i++)
			//{
			//	if (i == (pAdapter->AddressLength - 1))
			//	{
			//		//TRACE1("%.2X\n", (int) pAdapter->Address[i]);
			//		szTemp.Format(_T("%.2X\n"), (int) pAdapter->Address[i]);
			//		m_macaddress+=szTemp;
			//	}
			//	else
			//	{
			//		//TRACE1("%.2X-", (int) pAdapter->Address[i]);
			//		szTemp.Format(_T("%.2X-"), (int) pAdapter->Address[i]);
			//		m_macaddress+=szTemp;
			//	}
			//}
			//TRACE1("\tIndex: \t%d\n", pAdapter->Index);
			bool bEthernet=false;
			//TRACE0("\tType: \t");
			switch (pAdapter->Type)
			{
			case MIB_IF_TYPE_OTHER:
				//TRACE0("Other\n");
				break;
			case MIB_IF_TYPE_ETHERNET:
				bEthernet=true;
				//TRACE0("Ethernet\n");
				break;
			case MIB_IF_TYPE_TOKENRING:
				//TRACE0("Token Ring\n");
				break;
			case MIB_IF_TYPE_FDDI:
				//TRACE0("FDDI\n");
				break;
			case MIB_IF_TYPE_PPP:
				//TRACE0("PPP\n");
				break;
			case MIB_IF_TYPE_LOOPBACK:
				//TRACE0("Lookback\n");
				break;
			case MIB_IF_TYPE_SLIP:
				//TRACE0("Slip\n");
				break;
			default:
				//TRACE1("Unknown type %ld\n", pAdapter->Type);
				break;
			}
			if(bEthernet)
			{
				CStringW szTemp=_T("");
				for (i = 0; i < pAdapter->AddressLength; i++)
				{
					//if (i == (pAdapter->AddressLength - 1))
					//{
					//	//TRACE1("%.2X\n", (int) pAdapter->Address[i]);
					//	szTemp.Format(_T("%.2X"), (int) pAdapter->Address[i]);
					//	m_macaddress+=szTemp;
					//}
					//else
					//{
					//	//TRACE1("%.2X-", (int) pAdapter->Address[i]);
					//	szTemp.Format(_T("%.2X-"), (int) pAdapter->Address[i]);
					//	m_macaddress+=szTemp;
					//}

					szTemp.Format(_T("%.2X"), (int) pAdapter->Address[i]);
					m_macaddress+=szTemp;
				}
			}
			
			
			if(bEthernet)
			{
				pAdapter=NULL;
			}
			else
				pAdapter = pAdapter->Next;
		}
	}
	else
	{
	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return 0;


}



void CTlsDevice::GetDeviceInfo()
{
	if (pinfo!=NULL)
		delete (pinfo);
	unsigned  long nError;
	nError	=	GetAdaptersInfo(pinfo,&len);
	if (nError==0)
	{
		ParseData();			
	}
	if (nError==ERROR_NO_DATA)
	{
		//AfxMessageBox("No adapter information exists for the local computer");
	}
	if (nError==ERROR_NOT_SUPPORTED)
	{
		//AfxMessageBox("GetAdaptersInfo is not supported by the operating system running on the local computer");
	}
	if (nError==ERROR_BUFFER_OVERFLOW)
	{
		pinfo= (PIP_ADAPTER_INFO)malloc(len);
		nError	=	GetAdaptersInfo(pinfo,&len);
		if (nError==0)
		{
			ParseData();			
		}
	}

	return;
}
void CTlsDevice::ParseData()
{

	if (pinfo!=NULL)
	{
		m_macaddress.Format(_T("%02X:%02X:%02X:%02X:%02X:%02X"),pinfo->Address[0],pinfo->Address[1],pinfo->Address[2],pinfo->Address[3],pinfo->Address[4],pinfo->Address[5]);
		m_uniqueid.Format(_T("%02X%02X%02X%02X%02X%02X"),pinfo->Address[0],pinfo->Address[1],pinfo->Address[2],pinfo->Address[3],pinfo->Address[4],pinfo->Address[5]);
		m_description	=	pinfo->Description;
		m_type.Format(_T("%d"),pinfo->Type);

		PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);
		m_IpAddress	="";
		do 
		{
			m_IpAddress	+=	pAddressList->IpAddress.String;
			pAddressList = pAddressList->Next;
			if (pAddressList != NULL)
				m_IpAddress	+=_T("\r\n");
		} while (pAddressList != NULL);

		m_subnet.Format(_T("%s"),pinfo->IpAddressList.IpMask.String);
		m_gateway.Format(_T("%s"),pinfo->GatewayList.IpAddress.String);
		if (pinfo->HaveWins) 
			m_PrimaryWinsServer.Format(_T("%s"),pinfo->PrimaryWinsServer.IpAddress.String );
		else
			m_PrimaryWinsServer.Format(_T("%s"),_T("N/A") );
		if (pinfo->DhcpEnabled )
			m_dhcp.Format(_T("%s"),pinfo->DhcpServer.IpAddress.String );
		else
			m_dhcp.Format(_T("%s"),_T("N/A"));
		pinfo = pinfo->Next;
	}
	

}

CStringW CTlsDevice::GetMAC()
{
	CStringW sz = m_macaddress;
	return sz;
}

CStringW CTlsDevice::GetUniqueMacID()
{
	return m_uniqueid;
}

//=============================================================================
//bool CTlsDevice::IsSipVisible()
//{
//	SIPINFO si;
//  memset(&si, 0, sizeof(si));
//  si.cbSize = sizeof(si);
//  SHSipInfo(SPI_GETSIPINFO, 0, &si, 0);
//
//  return (si.fdwFlags | SIPF_ON) != 0;
//}

//=============================================================================
//void CTlsDevice::ShowSip(bool bShow)
//{
////  if(bShow == IsSipVisible())
////    return;
///*  
//	SIPINFO si;
//  memset(&si, 0, sizeof(si));
//  si.cbSize = sizeof(si);
//  SipGetInfo(&si); ////SHSipInfo(SPI_GETSIPINFO, 0, &si, 0);
//
//  if(bShow)
//    si.fdwFlags |= SIPF_ON;
//  else
//    si.fdwFlags = SIPF_OFF; // | SIPF_LOCKED;
//
//  SipSetInfo(&si);
//  //SHSipInfo(SPI_SETSIPINFO, 0, &si, 0);
//*/
//}



/*=============================================================================
    E O F
=============================================================================*/
