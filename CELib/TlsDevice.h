//=============================================================================

//=============================================================================

#ifndef __TLS_DEVICE_H__
#define __TLS_DEVICE_H__


#include <iphlpapi.h>

//===========================================================================//
class CTlsDevice  
{
public:
	CTlsDevice();
	virtual ~CTlsDevice();
	
	/*int         GetVersion();

	bool        IsInEmulator();
	
	void        GetDeviceBuild(CStringW& sBuild);	*/		//ok
	
	static CStringW		GetDeviceID();		
	static CStringW GetSerialNumberFromKernelIoControl();
	static void GetDeviceInfo();
	static int GetAdapterInfo();
	static void ParseData();
	static CStringW GetMAC();
	static CStringW GetUniqueMacID();
	//void		GetDeviceID(char* pszBuffer);
	//
	//BYTE        GetBatteryFlag();							//ok
	//BYTE        GetBackupBatteryLifePercent();			//ok
	//BYTE        GetBackupBatteryFlag();					//ok
	//DWORD       GetAvailableMemory();						//ok
	//BYTE		GetBatteryLifePercent();					//ok
	//ULONG		GetFreeFlashSpace();						//ok
	//bool        SetSysTime(DWORD dwNewTime);
	
	//void        DbgSetBatteryFlag(bool bPower);
	
	//void        BeepSuccess();
	//void        BeepQuestion();				//ok
	//void        BeepWarning();
	//void        BeepError();					//ok
	//void        BeepAkku();					//ok
	//
	/*bool        IsSipVisible();
	void        ShowSip(bool bShow);
	*/
	//static bool         RegisterComDll(CStringW sDll);
	
	//void        Reboot()=0;						//ok
	//void        Calibrate()=0;
	//void        PowerOff()=0;					//ok
	
	//void        SetLocalTime(CTlsTime tm)=0;

	//void		PreventSystemPowerDown(){};		//ok
	
	/*void		EnableRegFlush() {};
	void		DisableRegFlush() {};
	void		FlushReg() {};*/

	static PIP_ADAPTER_INFO pinfo;
	static unsigned long len;
	static CStringW	m_macaddress;
	static CStringW	m_uniqueid;
	static CStringW	m_description;
	static CStringW	m_type;
	static CStringW	m_subnet;
	static CStringW	m_IpAddress;
	static CStringW	m_gateway;
	static CStringW	m_PrimaryWinsServer;
	static CStringW	m_dhcp;
	
};



#endif
/*=============================================================================
    E O F
=============================================================================*/
