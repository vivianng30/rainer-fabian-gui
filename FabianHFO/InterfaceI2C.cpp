// InterfaceI2C.cpp : implementation file
//

#include "stdafx.h"
//#include "FabianHFO.h"
#include "InterfaceI2C.h"
#include "MVModel.h"
#include "TlsFile.h"
//#include "config.h"

extern CEvent g_eventI2Cdata;
extern CEvent g_eventI2Cdone;

/*- Brief Description -----------------------------------------------------*/
//  Handle communication operations on DIO interface
// 
/*- Preprocessor directives and compiler switches -------------------------*/
#define NI2CSHARE  FILE_SHARE_WRITE		          // Share mode
#define NI2CACCESS (GENERIC_READ | GENERIC_WRITE) // Access mode


#define I2C_IOCTL(device, code, inbuf, inlen, outbuf, outlen, actual) \
	DeviceIoControl(device, code, inbuf, inlen, outbuf, outlen, actual, NULL)

#define I2C_WAITTIME	20

CRITICAL_SECTION CInterfaceI2C::m_csI2C;




// **************************************************************************
// MPC23016 COMMANDS ////////////////////////////////////////////////////////
// PCA9555x ???
#define GP0_ACCESS		0
#define GP1_ACCESS		1
#define GP1_OLAT		3
#define GP0_IPOL		4
#define GP1_IPOL		5
#define GP0_DIR			6
#define GP1_DIR			7

//Maiboard 4.x
#define IOCON			0x0a
#define IODIRA			0x00
#define IODIRB			0x01
#define IPOLA			0x02
#define IPOLB			0x03
#define GPIOA			0x12
#define GPIOB			0x13
#define OLATA			0x14
#define OLATB			0x15

////MCP23016 //////////////////////////////////////////////////////////////////
////FM24CL64
//#define DEVICE_ADDRESS_MCP 0x20<<1  // A0...A2 ground
//#define DEVICE_ADDRESS_FRAM1 0x80<<1  // Bit7..4=devicetype=1010b, Bit3..1=address select bits, Bit0 = read/write Bit -> 1=read, 0=write

#define MAXFRAMADDRESS	0x1FFF

/////////////////////////////////////////////////////////////////////////////
// Device list
NI2C_MSG_HEADER scan_msg = {0, 0x00, 0x0000};
//NI2C_MSG_HEADER scan_msgMCP = {0, 0x00, 0x0000};
//NI2C_MSG_HEADER scan_msgFRAM1 = {0, 0x00, 0x0000};



CInterfaceI2C* CInterfaceI2C::theI2CInterface=0;

CInterfaceI2C::CInterfaceI2C(void)
{
	InitializeCriticalSection(&m_csI2C);

	m_dwLastI2CError=0;

	m_hI2C = INVALID_HANDLE_VALUE;
	m_bDoI2CThread=false;
	m_bI2C_FRAMavailability=FALSE;
	m_bI2C_ACCUavailability=FALSE;
	m_bI2C_MCPavailability=FALSE;

	m_iI2CErrorCodeBits=0;
	m_bI2CerrorPending=false;
	m_bAlarmSet=false;
	//m_pMsg=NULL;               // One write message, 2 bytes
	//m_pBuffer=NULL;
	//m_iBuffer=0;

	m_bWriteByte=true;
	m_bReadByte=true;
	m_chDevAddr=0;
	m_wRegister=0;
	m_chRegister1=0;
	m_chRegister2=0;
	m_chReturn1=0;
	m_chReturn2=0;
	m_chValue=0;
	m_wValue=0;

	m_byGP1_ACCESS=0;

	m_wUseForUDP=0;

	m_iMainBoard=MAINBOARD_21;

	m_bReadI2C=false;
	m_bLog=true;
	m_eMCPstate=MCP_NONE;
	m_bWatchdog=false;
	m_bFRAM=false;
	//m_bTREND=false;
	m_bACCU=false;
	/*scan_msg = {0, 0x00, 0x0000};*/
	scan_msg.chDevAddr = 0;
	scan_msg.chFlags = 0x00;
	scan_msg.wLen = 0x0000;

	m_byCurDevAddr=0; 
	m_wCurReg=0;

	m_pcwtI2CThread=NULL;
	m_hI2CThread=INVALID_HANDLE_VALUE;

	
}

CInterfaceI2C::~CInterfaceI2C(void)
{
	m_eMCPstate=MCP_NONE;
	m_bFRAM=false;
	m_bACCU=false;

	StopI2CThread();

	if(m_pcwtI2CThread!=NULL)
	{
		delete m_pcwtI2CThread;
		m_pcwtI2CThread=NULL;

		if(m_hI2CThread!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hI2CThread);
			m_hI2CThread=INVALID_HANDLE_VALUE;
		}
	}

	if( m_hI2C != INVALID_HANDLE_VALUE )
		CloseHandle( m_hI2C );
	m_hI2C = INVALID_HANDLE_VALUE;

	DeleteCriticalSection(&m_csI2C);
}

// **************************************************************************
// 
// **************************************************************************
CInterfaceI2C* CInterfaceI2C::GetInstance()
{
	if(theI2CInterface == 0)
	{
		theI2CInterface = new CInterfaceI2C;
	}
	return theI2CInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::DestroyInstance()
{
	if(theI2CInterface != NULL)
	{
		delete theI2CInterface;
		theI2CInterface = NULL;
	}
}



// **************************************************************************
// 
// **************************************************************************
bool CInterfaceI2C::Init(bool bStartup)
{
	bool bRes=true;
	m_hI2C = CreateFile(_T("I2C2:"), NI2CACCESS, NI2CSHARE, NULL,OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, 0);
	if(m_hI2C==INVALID_HANDLE_VALUE)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0107"));
		SetI2CErrorCode(ERRC_I2C_INIT);
		bRes=false;
	}
	else
	{
		StartI2CThread();

		/*if(ScanForDevice(DEVICE_ADDRESS_MCP))
		{
			InitMCP_ControlerPIC();
			SetI2C_MCPavailability(TRUE);
		}
		else
		{
			SetI2CErrorCode(ERRC_I2C_MCP);
		}*/

		if(ScanForDevice(DEVICE_ADDRESS_CONFIG,bStartup))
		{
			SetFRAMstate(true);
			SetI2C_FRAMavailability(TRUE);
		}
		else
		{
			SetI2CErrorCode(ERRC_I2C_FRAM);
			bRes=false;
		}

		if(ScanForDevice(DEVICE_ADDRESS_ACCUPIC,bStartup))
		{
			SetACCUstate(true);
			SetI2C_ACCUavailability(TRUE);
		}
		else
		{
			SetACCUstate(false);
			SetI2C_ACCUavailability(FALSE);
			SetI2CErrorCode(ERRC_I2C_ACCU);
			bRes=false;
		}

		scanForDeviceMCP(bStartup);
	}
	
	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceI2C::Deinit()
{
	m_eMCPstate=MCP_NONE;
	m_bFRAM=false;
	m_bACCU=false;

	StopI2CThread();

	if(m_pcwtI2CThread!=NULL)
	{
		delete m_pcwtI2CThread;
		m_pcwtI2CThread=NULL;

		if(m_hI2CThread!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hI2CThread);
			m_hI2CThread=INVALID_HANDLE_VALUE;
		}
	}



	if( m_hI2C != INVALID_HANDLE_VALUE )
		CloseHandle( m_hI2C );
	m_hI2C = INVALID_HANDLE_VALUE;

	return true;
}

void CInterfaceI2C::scanForDeviceMCP(bool bStartup)
{
	if(ScanForDevice(DEVICE_ADDRESS_MCP,bStartup))
	{
		if(TRUE==IsI2C_FRAMavailability())
		{
			m_iMainBoard=ReadConfigByte(HWCONF_MAINBOARD);
		}

		bool bResult=InitMCP_ControlerPIC();
		
		/*if(m_iMainBoard<MAINBOARD_40)
		{
			bResult=InitMCP_ControlerPIC_MB3();
		}
		else
		{
			bResult=InitMCP_ControlerPIC_MB4();
		}*/
		if(bResult)
			SetI2C_MCPavailability(TRUE);
	}
	else
	{
		SetI2CErrorCode(ERRC_I2C_MCP);
	}
}


// **************************************************************************
// 
// **************************************************************************
bool CInterfaceI2C::ScanForDevice(BYTE bDeviceAddress,bool bStartup)
{

	if(m_hI2C==INVALID_HANDLE_VALUE)
	{
		//theApp.getLog()->WriteLine(_T("#HFO:0108"));
		return false;
	}
	EnterCriticalSection(&m_csI2C);
	scan_msg.chDevAddr=bDeviceAddress;


	// Scan I2C bus for device //////////////////////////////////////////////
	I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (BYTE*)&scan_msg, sizeof(scan_msg), NULL, 0, NULL); 
	I2C_IOCTL(m_hI2C, IOCTL_NI2C_GET_RESULT, (BYTE*)&scan_msg, sizeof(scan_msg), NULL, 0, NULL); 


	if (scan_msg.chFlags > 1)
	{
		LeaveCriticalSection(&m_csI2C);
		Sleep(5);
		//theApp.getLog()->WriteLine(_T("#HFO:0109"));
		m_bI2CerrorPending=SetI2Cerror(bDeviceAddress,bStartup);
		return false;
	}
	else
	{
		LeaveCriticalSection(&m_csI2C);
	}
	
	return true;
}
bool CInterfaceI2C::InitMCP_ControlerPIC()
{

	bool bResult=false;
	if(m_iMainBoard<MAINBOARD_40)
	{
		bResult=InitMCP_ControlerPIC_MB3();
	}
	else
	{
		bResult=InitMCP_ControlerPIC_MB4();
	}

	if(bResult)
		SetMCPstate(MCP_CONTROLLER);

	return bResult;
}
// **************************************************************************
// MB3.1
// **************************************************************************
bool CInterfaceI2C::InitMCP_ControlerPIC_MB3()
{
	BYTE b0=0xff;

	m_eMCPstate=MCP_CONTROLLER;
	EnterCriticalSection(&m_csI2C);
	// Port0    /////////////////////////////////////////////////////////////
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_DIR, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_ACCESS, b0)==false)	// IO not in use set to 1 = write
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}


	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_DIR, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	m_byGP1_ACCESS=0x10;//aktiviert Konverter rs232 zu TTL;

	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_ACCESS, m_byGP1_ACCESS)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_IPOL, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_IPOL, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}
	LeaveCriticalSection(&m_csI2C);

	return true;
}
 
// **************************************************************************
// MB4
// **************************************************************************
bool CInterfaceI2C::InitMCP_ControlerPIC_MB4()
{
	BYTE b0=0xff;

	m_eMCPstate=MCP_CONTROLLER;
	EnterCriticalSection(&m_csI2C);
	// Port0    /////////////////////////////////////////////////////////////
	if(SetRegister8(DEVICE_ADDRESS_MCP, IODIRA, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, OLATA, b0)==false)	// IO not in use set to 1 = write
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}


	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, IODIRB, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	m_byGP1_ACCESS=0x10;//aktiviert Konverter rs232 zu TTL;

	if(SetRegister8(DEVICE_ADDRESS_MCP, OLATB, m_byGP1_ACCESS)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, IPOLA, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, IPOLB, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}
	LeaveCriticalSection(&m_csI2C);

	return true;
}
bool CInterfaceI2C::InitMCP_HfoPIC()
{
	bool bResult=false;
	if(m_iMainBoard<MAINBOARD_40)
	{
		bResult=InitMCP_HfoPIC_MB3();
	}
	else
	{
		bResult=InitMCP_HfoPIC_MB4();
	}

	if(bResult)
		SetMCPstate(MCP_HFO);

	return bResult;
}

bool CInterfaceI2C::InitMCP_HfoPIC_MB3()
{
	BYTE b0=0xff;
	m_eMCPstate=MCP_HFO;

	EnterCriticalSection(&m_csI2C);

	// Port0    /////////////////////////////////////////////////////////////
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_DIR, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_ACCESS, b0)==false)	// IO not in use set to 1 = write
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	//b0=0xff;
	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_DIR, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	m_byGP1_ACCESS=0x18;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_ACCESS, m_byGP1_ACCESS)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_IPOL, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_IPOL, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}
	LeaveCriticalSection(&m_csI2C);
	return true;

}
bool CInterfaceI2C::InitMCP_HfoPIC_MB4()
{
	BYTE b0=0xff;
	m_eMCPstate=MCP_HFO;

	EnterCriticalSection(&m_csI2C);

	// Port0    /////////////////////////////////////////////////////////////
	if(SetRegister8(DEVICE_ADDRESS_MCP, IODIRA, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, OLATA, b0)==false)	// IO not in use set to 1 = write
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	//b0=0xff;
	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, IODIRB, b0)==false)		// Input=1;
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	m_byGP1_ACCESS=0x18;
	if(SetRegister8(DEVICE_ADDRESS_MCP, OLATB, m_byGP1_ACCESS)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, IPOLA, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();;
		return false;
	}

	b0=0;
	if(SetRegister8(DEVICE_ADDRESS_MCP, IPOLB, b0)==false)
	{
		LeaveCriticalSection(&m_csI2C);
		ReinitI2C();
		return false;
	}
	LeaveCriticalSection(&m_csI2C);

	return true;
}
//bool CInterfaceI2C::InitMCP_HfoPIC()
//{
//	BYTE b0=0xff;
//
//	m_eMCPstate=MCP_HFO;
//
//	EnterCriticalSection(&m_csI2C);
//	// Port0    /////////////////////////////////////////////////////////////
//	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_DIR, b0)==false)		// Input=1;
//	{
//		LeaveCriticalSection(&m_csI2C);
//		//theApp.getLog()->WriteLine(_T("ERROR: GP0_DIR_HFO"));
//		ReinitI2C();
//		return false;
//	}
//
//	b0=0;
//	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_ACCESS, b0)==false)	// IO not in use set to 1 = write
//	{
//		LeaveCriticalSection(&m_csI2C);
//		//theApp.getLog()->WriteLine(_T("ERROR: GP0_ACCESS_HFO"));
//		ReinitI2C();
//		return false;
//	}
//
//	//b0=0xff;
//	b0=0;
//	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_DIR, b0)==false)		// Input=1;
//	{
//		LeaveCriticalSection(&m_csI2C);
//		//theApp.getLog()->WriteLine(_T("ERROR: GP1_DIR_HFO"));
//		ReinitI2C();
//		return false;
//	}
//
//	m_byGP1_ACCESS=8;
//	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_ACCESS, m_byGP1_ACCESS)==false)
//	{
//		LeaveCriticalSection(&m_csI2C);
//		//theApp.getLog()->WriteLine(_T("ERROR: GP1_ACCESS_HFO"));
//		ReinitI2C();
//		return false;
//	}
//
//	b0=0;
//	if(SetRegister8(DEVICE_ADDRESS_MCP, GP0_IPOL, b0)==false)
//	{
//		LeaveCriticalSection(&m_csI2C);
//		//theApp.getLog()->WriteLine(_T("ERROR: GP0_IPOL_HFO"));
//		ReinitI2C();
//		return false;
//	}
//
//	b0=0;
//	if(SetRegister8(DEVICE_ADDRESS_MCP, GP1_IPOL, b0)==false)
//	{
//		LeaveCriticalSection(&m_csI2C);
//		//theApp.getLog()->WriteLine(_T("ERROR: GP1_IPOL_HFO"));
//		ReinitI2C();
//		return false;
//	}
//
//	LeaveCriticalSection(&m_csI2C);
//	//SetMCPstate(true);
//
//	return true;
//}

void CInterfaceI2C::SetMCPwatchdog()
{
	if(m_eMCPstate!=MCP_NONE)
	{
		//BYTE b0=0;
		BYTE b1=1;

		EnterCriticalSection(&m_csI2C);

		m_bWatchdog=!m_bWatchdog;
		if(m_bWatchdog)
		{
			m_byGP1_ACCESS=m_byGP1_ACCESS|b1;
		}
		else
		{
			m_byGP1_ACCESS = m_byGP1_ACCESS & ~b1;
		}

		if(m_iMainBoard<MAINBOARD_40)
			SetRegister8(DEVICE_ADDRESS_MCP, GP1_ACCESS, m_byGP1_ACCESS);
		else
			SetRegister8(DEVICE_ADDRESS_MCP, OLATB, m_byGP1_ACCESS);

		LeaveCriticalSection(&m_csI2C);
	}
}

BYTE CInterfaceI2C::GetByteMCPwatchdog()
{
	return m_chReturn1;
}

void CInterfaceI2C::SetMCPstate(eMCPstate state)
{
	m_eMCPstate=state;
}
eMCPstate CInterfaceI2C::GetMCPstate()
{
	return m_eMCPstate;
}
void CInterfaceI2C::SetFRAMstate(bool state)
{
	m_bFRAM=state;
}

void CInterfaceI2C::SetACCUstate(bool state)
{
	m_bACCU=state;
	/*if(state)
		theApp.getLog()->WriteLine(_T("***CInterfaceI2C::SetACCUstate true"));
	else
		theApp.getLog()->WriteLine(_T("***CInterfaceI2C::SetACCUstate false"));*/
}
bool CInterfaceI2C::GetACCUstate()
{
	return m_bACCU;
}


bool CInterfaceI2C::Erase64FRAM()
{

	//TEST
	/*WriteConfigByte(0x1EFB,1);
	WriteConfigByte(0x1EFC,2);
	WriteConfigByte(0x1EFD,3);
	WriteConfigByte(0x1EFE,4);
	WriteConfigByte(HWCONF_HFOSPRING,5);*/



	// Adressbereich 0x1EFF bis 0x1FFF ist für Hardware-Konfiguration reserviert, nicht löschen!!!!!

	bool bReturn=true;
	NI2C_MSG_HEADER msg[1];               // One write message, 2 bytes
	//BYTE buffer[8193];
	//BYTE buffer[2050] = {0};
	BYTE buffer[1985] = {0};

	msg[0].chDevAddr = DEVICE_ADDRESS_CONFIG;
	msg[0].chFlags = 0;
	//msg[0].wLen = 8193;
	//msg[0].wLen = 2050;
	msg[0].wLen = 1985;
	
	WORD wReg =0;
	int iBlocks=0;
	
	for (iBlocks;iBlocks<4;iBlocks++)
	{
		wReg=iBlocks*1983;

		buffer[0] = HIBYTE(wReg);                  
		buffer[1] = LOBYTE(wReg);

		/*for(i=0;i<2048;i++)
		{
			buffer[i+2] = 0;
		}*/

		EnterCriticalSection(&m_csI2C);

		if(!I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msg, sizeof(msg), buffer, sizeof(buffer), NULL))
			bReturn=false;
		if (msg[0].chFlags > 1)
		{
			Sleep(0);
			m_bI2CerrorPending=SetI2Cerror(DEVICE_ADDRESS_CONFIG);
		}
		if(!I2C_IOCTL(m_hI2C, IOCTL_NI2C_SKIP_RESULT, (LPBYTE)msg, sizeof(msg),buffer, sizeof(buffer), NULL))
			bReturn=false;
		if (msg[0].chFlags > 1)
		{
			Sleep(0);
			m_bI2CerrorPending=SetI2Cerror(DEVICE_ADDRESS_CONFIG);
		}

		LeaveCriticalSection(&m_csI2C);

		Sleep(1);
		
	
	}

	//TEST
	/*int iTest1=ReadConfigByte(0x1EFB);
	int iTest2=ReadConfigByte(0x1EFC);
	int iTest3=ReadConfigByte(0x1EFD);
	int iTest4=ReadConfigByte(0x1EFE);
	int iTest5=ReadConfigByte(HWCONF_HFOSPRING);*/

	return bReturn;
}


bool CInterfaceI2C::SetFRAMDefaultFactoryValues()
{
	bool bResult=true;

	WriteConfigWord(PARA_ITIME_TRIGGER_16,FACTORY_IPPV_ITIME);
	WriteConfigWord(PARA_ETIME_TRIGGER_16,FACTORY_IPPV_ETIME);
	WriteConfigWord(PARA_BPM_TRIGGER_16,FACTORY_IPPV_BPM);
	WriteConfigWord(PARA_IFLOW_TRIGGER_16,FACTORY_IFLOW);
	WriteConfigWord(PARA_EFLOW_TRIGGER_16,FACTORY_EFLOW);
	WriteConfigWord(PARA_PEEP_TRIGGER_16,FACTORY_PEEP);
	WriteConfigWord(PARA_PINSP_TRIGGER_16,FACTORY_PINSP);
	WriteConfigWord(PARA_PMAXVOLG_TRIGGER_16,FACTORY_PMAXVOLG);
	WriteConfigWord(PARA_RISETIME_TRIGGER_16,FACTORY_RISETIME);

	WriteConfigWord(PARA_ITIME_IPPV_16,FACTORY_IPPV_ITIME);
	WriteConfigWord(PARA_ITIME_NMODE_16,FACTORY_NMODE_ITIME);
	WriteConfigWord(PARA_ETIME_IPPV_16,FACTORY_IPPV_ETIME);
	WriteConfigWord(PARA_ETIME_NMODE_16,FACTORY_NMODE_ETIME);
	WriteConfigWord(PARA_BPM_IPPV_16,FACTORY_IPPV_BPM);
	WriteConfigWord(PARA_BPM_NMODE_16,FACTORY_NMODE_BPM);
	WriteConfigWord(PARA_RISETIME_IPPV_16,FACTORY_RISETIME);
	WriteConfigWord(PARA_IFLOW_IPPV_16,FACTORY_IFLOW);
	WriteConfigWord(PARA_EFLOW_IPPV_16,FACTORY_EFLOW);
	WriteConfigWord(PARA_THERAPYFLOW_16,FACTORY_THERAPYFLOW);
	WriteConfigWord(PARA_FLOWMIN_16,FACTORY_FLOWMIN);
	WriteConfigWord(PARA_PEEP_IPPV_16,FACTORY_PEEP);
	WriteConfigWord(PARA_CPAP_16,FACTORY_CPAP);
	WriteConfigWord(PARA_CPAP_NMODE_16,FACTORY_CPAP);
	WriteConfigWord(PARA_PINSP_IPPV_16,FACTORY_PINSP);
	WriteConfigWord(PARA_PMAXVOLG_IPPV_16,FACTORY_PMAXVOLG);
	WriteConfigWord(PARA_PPSV_16,FACTORY_PEEP+20);
	WriteConfigWord(PARA_PMAN_HFO_16,FACTORY_PMAN);
	WriteConfigWord(PARA_PMAN_CPAP_16,FACTORY_PMAN);
	WriteConfigWord(PARA_PMAN_NMODE_16,FACTORY_PMAN);
	WriteConfigWord(PARA_O2_16,FACTORY_O2);
	WriteConfigWord(PARA_O2FLUSH_16,FACTORY_O2FLUSH);
	WriteConfigWord(PARA_O2DIFF_16,MAXDIFF_O2_FLUSH);
	WriteConfigWord(PARA_PMEANDIFF_16,MAXDIFF_PMEANREC);
	WriteConfigWord(PARA_VLIMIT_TRIGGER_16,FACTORY_VLIMIT);
	WriteConfigByte(PARA_VLIMITSTATE_TRIGGER_8,FACTORY_VLIMITSTATE);
	WriteConfigWord(PARA_VGARANT_TRIGGER_16,FACTORY_VGARANT);
	WriteConfigByte(PARA_VGARANTSTATE_TRIGGER_8,FACTORY_VGARANTSTATE);

	WriteConfigWord(PARA_VLIMIT_IPPV_16,FACTORY_VLIMIT);
	WriteConfigByte(PARA_VLIMITSTATE_IPPV_8,FACTORY_VLIMITSTATE);
	WriteConfigWord(PARA_VGARANT_IPPV_16,FACTORY_VGARANT);
	WriteConfigByte(PARA_VGARANTSTATE_IPPV_8,FACTORY_VGARANTSTATE);

	WriteConfigWord(PARA_TRIGGER_CONVENT_16,FACTORY_SIPPV_TRIGGER);
	WriteConfigWord(PARA_TRIGGER_CPAP_16,FACTORY_CPAP_TRIGGER);
	WriteConfigWord(PARA_TRIGGER_DUOPAP_16,FACTORY_DUOPAP_TRIGGER);
	WriteConfigWord(PARA_TRIGGER_NCPAP_16,FACTORY_NCPAP_TRIGGER);

	WriteConfigWord(PARA_BACKUP_16,FACTORY_CPAP_BACKUP);

	WriteConfigWord(PARA_HFO_HFAMPL_16,FACTORY_HFO_HFAMPL);
	WriteConfigWord(PARA_HFO_HFAMPL_VOLG_16,FACTORY_HFO_HFAMPL);
	WriteConfigWord(PARA_HFO_HFFREQ_16,FACTORY_HFO_HFFREQ);
	WriteConfigWord(PARA_HFO_PMEAN_16,FACTORY_HFO_PMEAN);
	WriteConfigByte(PARA_HFO_IERATIO_8,FACTORY_HFO_IERATIO);
	WriteConfigWord(PARA_HFO_VGARANT_16,FACTORY_HFO_VGARANT);
	WriteConfigByte(PARA_HFO_VGARANTSTATE_8,FACTORY_HFO_VGARANTSTATE);

	WriteConfigWord(PARA_HFO_ITIMEREC16,FACTORY_HFO_ITIMEREC);
	WriteConfigWord(PARA_HFO_FREQREC16,FACTORY_HFO_FREQREC);
	WriteConfigWord(PARA_HFO_PMEANREC16,FACTORY_HFO_PMEANREC);

	
	WriteConfigWord(ALIMIT_VAL_MVMAX_16,FACTORY_ALIMIT_VAL_MVMAX);
	WriteConfigByte(ALIMIT_STATE_MVMAX_8,FACTORY_ALIMIT_STATE_MVMAX);
	WriteConfigWord(ALIMIT_VAL_MVMIN_16,FACTORY_ALIMIT_VAL_MVMIN);
	WriteConfigByte(ALIMIT_STATE_MVMIN_8,FACTORY_ALIMIT_STATE_MVMIN);
	WriteConfigWord(ALIMIT_VAL_PIPMAX_16,FACTORY_ALIMIT_VAL_PIPMAX);
	WriteConfigByte(ALIMIT_STATE_PIPMAX_8,FACTORY_ALIMIT_STATE_PIPMAX);
	WriteConfigWord(ALIMIT_VAL_PIPMIN_16,FACTORY_ALIMIT_VAL_PIPMIN);
	WriteConfigByte(ALIMIT_STATE_PIPMIN_8,FACTORY_ALIMIT_STATE_PIPMIN);
	WriteConfigWord(ALIMIT_VAL_PEEP_16,FACTORY_ALIMIT_VAL_PEEP);
	WriteConfigByte(ALIMIT_STATE_PEEP_8,FACTORY_ALIMIT_STATE_PEEP);
	WriteConfigWord(ALIMIT_VAL_BPMMAX_16,FACTORY_ALIMIT_VAL_BPMMAX);
	WriteConfigByte(ALIMIT_STATE_BPMMAX_8,FACTORY_ALIMIT_STATE_BPMMAX);
	WriteConfigWord(ALIMIT_VAL_LEAKMAX_16,FACTORY_ALIMIT_VAL_LEAKMAX);
	WriteConfigByte(ALIMIT_STATE_LEAKMAX_8,FACTORY_ALIMIT_STATE_LEAKMAX);
	WriteConfigWord(ALIMIT_VAL_APNOE_16,FACTORY_ALIMIT_VAL_APNOE);
	WriteConfigByte(ALIMIT_STATE_APNOE_8,FACTORY_ALIMIT_STATE_APNOE);

	WriteConfigWord(ALIMIT_VAL_MVMAX_HF_16,FACTORY_ALIMIT_VAL_MVMAX);
	WriteConfigByte(ALIMIT_STATE_MVMAX_HF_8,FACTORY_ALIMIT_STATE_MVMAX);
	WriteConfigWord(ALIMIT_VAL_MVMIN_HF_16,FACTORY_ALIMIT_VAL_MVMIN_HF);
	WriteConfigByte(ALIMIT_STATE_MVMIN_HF_8,FACTORY_ALIMIT_STATE_MVMIN);
	WriteConfigWord(ALIMIT_VAL_PMAX_HF_16,FACTORY_ALIMIT_VAL_PIPMAX);
	WriteConfigByte(ALIMIT_STATE_PMAX_HF_8,FACTORY_ALIMIT_STATE_PIPMAX);
	WriteConfigWord(ALIMIT_VAL_PEEP_HF_16,FACTORY_ALIMIT_VAL_PEEP);
	WriteConfigByte(ALIMIT_STATE_PEEP_HF_8,FACTORY_ALIMIT_STATE_PEEP);
	WriteConfigWord(ALIMIT_VAL_BPMMAX_HF_16,FACTORY_ALIMIT_VAL_BPMMAX);
	WriteConfigByte(ALIMIT_STATE_BPMMAX_HF_8,FACTORY_ALIMIT_STATE_BPMMAX);
	WriteConfigWord(ALIMIT_VAL_LEAKMAX_HF_16,FACTORY_ALIMIT_VAL_LEAKMAX);
	WriteConfigByte(ALIMIT_STATE_LEAKMAX_HF_8,FACTORY_ALIMIT_STATE_LEAKMAX);
	WriteConfigWord(ALIMIT_VAL_APNOE_HF_16,FACTORY_ALIMIT_VAL_APNOE_HF);
	WriteConfigByte(ALIMIT_STATE_APNOE_HF_8,FACTORY_ALIMIT_STATE_APNOE_HF);

	WriteConfigWord(ALIMIT_VAL_PMAX_CPAP_16,FACTORY_ALIMIT_VAL_PIPMAX);
	WriteConfigWord(ALIMIT_VAL_PEEP_CPAP_16,FACTORY_ALIMIT_VAL_PEEP);
	WriteConfigByte(ALIMIT_STATE_PMAX_CPAP_8,FACTORY_ALIMIT_STATE_PIPMAX);
	WriteConfigByte(ALIMIT_STATE_PEEP_CPAP_8,FACTORY_ALIMIT_STATE_PEEP);

	WriteConfigWord(ALIMIT_VAL_PMAX_NCPAP_16,FACTORY_ALIMIT_VAL_PIPMAX);
	WriteConfigWord(ALIMIT_VAL_PEEP_NCPAP_16,FACTORY_ALIMIT_VAL_PEEP);
	WriteConfigByte(ALIMIT_STATE_PMAX_NCPAP_8,FACTORY_ALIMIT_STATE_PIPMAX);
	WriteConfigByte(ALIMIT_STATE_PEEP_NCPAP_8,FACTORY_ALIMIT_STATE_PEEP);

	WriteConfigWord(ALIMIT_VAL_PMAX_DUOPAP_16,FACTORY_ALIMIT_VAL_PIPMAX);
	WriteConfigWord(ALIMIT_VAL_PEEP_DUOPAP_16,FACTORY_ALIMIT_VAL_PEEP);
	WriteConfigByte(ALIMIT_STATE_PMAX_DUOPAP_8,FACTORY_ALIMIT_STATE_PIPMAX);
	WriteConfigByte(ALIMIT_STATE_PEEP_DUOPAP_8,FACTORY_ALIMIT_STATE_PEEP);
	WriteConfigWord(PARA_PPSVDIFF_16,DIFF_PEEP_PINSP);

	


	return bResult;
}


bool CInterfaceI2C::SetFRAMDefaultFactoryConfig()
{
	bool bResult=true;

	//save demo licneses and restore afterwards
	COleDateTime dtFOTdemoTimestamp = getModel()->getCONFIG()->GetFOTdemoTimestamp();
	COleDateTime dtPRICOdemoTimestamp = getModel()->getCONFIG()->GetPRICOdemoTimestamp();
	COleDateTime dtTHERAPYdemoTimestamp = getModel()->getCONFIG()->GetTHERAPYdemoTimestamp();
	COleDateTime dtTRENDdemoTimestamp = getModel()->getCONFIG()->GetTRENDdemoTimestamp();
	COleDateTime dtLUNGRECdemoTimestamp = getModel()->getCONFIG()->GetLUNGRECdemoTimestamp();
	COleDateTime dtVLIMITdemoTimestamp = getModel()->getCONFIG()->GetVLIMITdemoTimestamp();
	COleDateTime dtVGUARANTYdemoTimestamp = getModel()->getCONFIG()->GetVGUARANTYdemoTimestamp();
	COleDateTime dtNMODEdemoTimestamp = getModel()->getCONFIG()->GetNMODEdemoTimestamp();
	COleDateTime dtHFOdemoTimestamp = getModel()->getCONFIG()->GetHFOdemoTimestamp();

	Erase64FRAM();

	//restore demo license timestamp
	getModel()->getCONFIG()->SetFOTdemoTimestamp(dtFOTdemoTimestamp);
	getModel()->getCONFIG()->SetPRICOdemoTimestamp(dtPRICOdemoTimestamp);
	getModel()->getCONFIG()->SetTHERAPYdemoTimestamp(dtTHERAPYdemoTimestamp);
	getModel()->getCONFIG()->SetTRENDdemoTimestamp(dtTRENDdemoTimestamp);
	getModel()->getCONFIG()->SetLUNGRECdemoTimestamp(dtLUNGRECdemoTimestamp);
	getModel()->getCONFIG()->SetVLIMITdemoTimestamp(dtVLIMITdemoTimestamp);
	getModel()->getCONFIG()->SetVGUARANTYdemoTimestamp(dtVGUARANTYdemoTimestamp);
	getModel()->getCONFIG()->SetNMODEdemoTimestamp(dtNMODEdemoTimestamp);
	getModel()->getCONFIG()->SetHFOdemoTimestamp(dtHFOdemoTimestamp);

	WriteConfigByte(SPO2FASTSAT_8, 1);
	WriteConfigByte(SPO2SENSITIVITY_8, SPO2_SENSITIVITY_NORMAL);
	WriteConfigWord(CO2BAROPRESSURE_16, 760);
	WriteConfigByte(CO2TIMEPUMPAUTOON, (BYTE)10);
	//WriteConfigByte(ETCO2AVG_8, (BYTE)xxx);
	WriteConfigByte(CO2UNIT_8,(BYTE)0);
	WriteConfigByte(FLOWSENSORSTATE_8,FLOWSENSOR_ON);
	WriteConfigByte(NURSECALLACTIVE_8, (BYTE)0);
	WriteConfigByte(FACTORYSETTINGS_8, (BYTE)0);
	WriteConfigByte(AUTOALARMLIMIT_8, (BYTE)0);
	WriteConfigByte(SERVICEPSWRDDISABLED_8, (BYTE)0);
	//WriteConfigByte(LANGUAGEID_8,0);
	WriteConfigByte(VENTRANGE_8,NEONATAL);
	WriteConfigByte(CURVENTMODE_8,VM_IPPV);
	WriteConfigByte(PREVVENTMODE_8,VM_IPPV);
	WriteConfigByte(LASTTRIGGERMODE_8,VM_SIPPV);
	WriteConfigByte(LASTNMODE_8,VM_NCPAP);
	WriteConfigByte(ALARMVOLUME_8,ALARMMEDIUM);

	WriteConfigWord(PARA_FOT_CONV_AMPLITUDE_16,FACTORY_FOT_CONV_AMPL);
	WriteConfigByte(PARA_FOT_CONV_FREQ_8,FACTORY_FOT_CONV_FREQ);
	WriteConfigByte(PARA_FOT_CONV_STEPS_8,FACTORY_FOT_CONV_STEPS);
	WriteConfigWord(PARA_FOT_CONV_PEEPSTART_16,FACTORY_FOT_CONV_PEEP_START);
	WriteConfigWord(PARA_FOT_CONV_PEEPEND_16,FACTORY_FOT_CONV_PEEP_END);

	WriteConfigWord(PARA_FOT_HFO_AMPLITUDE_16,FACTORY_FOT_HFO_AMPL);
	WriteConfigByte(PARA_FOT_HFO_FREQ_8,FACTORY_FOT_HFO_FREQ);
	WriteConfigByte(PARA_FOT_HFO_STEPS_8,FACTORY_FOT_HFO_STEPS);
	WriteConfigWord(PARA_FOT_HFO_PMEANSTART_16,FACTORY_FOT_HFO_PMEAN_START);
	WriteConfigWord(PARA_FOT_HFO_PMEANEND_16,FACTORY_FOT_HFO_PMEAN_END);

	WriteConfigByte(MANBREATHTIME_8,10);
	WriteConfigByte(NEBULIZERTIME_8,10);
	WriteConfigByte(PMINALARMDELAY_8,5);
	WriteConfigByte(PATIENTALARMDELAY_8,10);
	WriteConfigByte(O2FLUSHTIME_8,120);
	WriteConfigByte(REFFLOWSENSOR_8,RF_NTPD);
	WriteConfigByte(TUBESET_8,TUBE_INFANTFLOW);
	//WriteConfigByte(ASKFORPATDATA_8,0);
	WriteConfigByte(USETVEBTB_8,0);
	WriteConfigByte(USENEOPED_8,0);
	WriteConfigByte(BTPS_8,1);
	WriteConfigByte(FRESHGASEXT_8, 0);
	WriteConfigByte(TRIGGERTYPECONV_8,TRIGGER_FLOW);

	WriteConfigByte(TRIGGERTYPENCPAP_8,TRIGGER_FLOW);
	WriteConfigByte(TRIGGERTYPEDUOPAP_8,TRIGGER_FLOW);
	WriteConfigByte(TRIGGERTYPECPAP_8,TRIGGER_FLOW);

	WriteConfigByte(EFLOWSETTINGS_8, 0);
	//WriteConfigByte(PLAYTRIGSOUND_8,0);
	WriteConfigByte(HFOMANBREATH_8,0);
	//WriteConfigByte(BREATHVOLGARANTY_8,5);
	WriteConfigByte(ABORTCRITERIONPSV_8,15);
	WriteConfigByte(AUTOOXYCAL_8, CAL_21_100);
	WriteConfigByte(FILLEDGRAPH_8,1);
	WriteConfigByte(FILLEDTREND_8,1);
	WriteConfigByte(GRAPHAUTOSCALE_8,1);
	WriteConfigByte(TRENDAUTOSCALE_8,1);
	WriteConfigByte(GRAPHLASTVIEWSUBSTATE_8,2);
	WriteConfigByte(GRAPHSPEEDNORM_8,G_SPEED_9950MS);
	WriteConfigByte(GRAPHSPEEDCLFIO2_8,G_SPEED_9950MS);
	WriteConfigByte(GRAPHSPEEDHFO_8,G_SPEED_1000MS);
	WriteConfigByte(CURLINEDIAGRAMM_8,IDC_LINEDIAGRAM);
	WriteConfigByte(TREND_TYPE1_8,TREND_PINSP);
	WriteConfigByte(TREND_TYPE2_8,TREND_PMEAN);
	WriteConfigByte(TREND_TYPE3_8,TREND_FIO2);
	WriteConfigWord(TREND_SPAN_16,TREND_16HOURS);
	WriteConfigWord(TRENDSCALE_PINSP_16,G_UPPER_MAXSCALE_PRESSURE);
	WriteConfigWord(TRENDSCALE_PMEAN_16,G_UPPER_MAXSCALE_PRESSURE);
	WriteConfigWord(TRENDSCALE_FIO2_16,G_UPPER_MAXSCALE_FIO2);
	WriteConfigWord(TRENDSCALE_VTE_16,G_UPPER_MAXSCALE_VTE);
	WriteConfigWord(TRENDSCALE_COMPLIANCE_16,G_UPPER_MAXSCALE_COMPL);
	WriteConfigWord(TRENDSCALE_DCO2_16,G_UPPER_MAXSCALE_DCO2);
	WriteConfigByte(ALIMITLASTNUMERICSTATE_8,ALIMITNUM_VENTILATION);
	WriteConfigByte(PRESSUREUNIT_8, (int)PUNIT_MBAR);
	WriteConfigByte(VOLUMECONTROL_8, (int)VCTRL_VGARANT);

	/*WriteConfigWord(MAXSCALE_PRESSURE_16,G_UPPER_MAXSCALE_PRESSURE);
	WriteConfigWord(MAXSCALE_VOLUME_16,G_UPPER_MAXSCALE_VOLUME);
	WriteConfigWord(MAXSCALE_FLOW_16,G_UPPER_MAXSCALE_FLOW);
	WriteConfigWord(MAXSCALE_PRESSUREHF_16,G_UPPER_MAXSCALE_PRESSURE);
	WriteConfigWord(MAXSCALE_VOLUMEHF_16,G_UPPER_MAXSCALE_VOLUME);
	WriteConfigWord(MAXSCALE_FLOWHF_16,G_UPPER_MAXSCALE_FLOW);
	WriteConfigWord(MINSCALE_PRESSUREHF_16,G_UPPER_MAXSCALE_PRESSURE/((150-10)*(-10)));
	WriteConfigWord(MINSCALE_VOLUMEHF_16,G_UPPER_MAXSCALE_VOLUME/((150-10)*(-10)));
	WriteConfigWord(MINSCALE_FLOWHF_16,G_UPPER_MAXSCALE_FLOW/((150-10)*(-10)));*/

	WriteConfigWord(MINSCALE_FOT_XRS_16,(G_LOWER_MINSCALE_XRS_FOT *(-1)));
	WriteConfigWord(G_LOWER_MAXSCALE_PRESSURE_FOT,G_UPPER_MAXSCALE_PRESSURE);
	WriteConfigWord(MAXSCALE_SPO2_HFGRAPH_16,G_UPPER_MAXSCALE_SPO2*10);
	WriteConfigWord(MAXSCALE_SPO2_GRAPH_16,G_UPPER_MAXSCALE_SPO2*10);
	WriteConfigWord(MAXSCALE_CO2_HFGRAPH_16,G_UPPER_MAXSCALE_CO2*10);
	WriteConfigWord(MAXSCALE_CO2_GRAPH_16,G_UPPER_MAXSCALE_CO2*10);
	WriteConfigWord(MAXSCALE_FLOW_GRAPH_16,G_UPPER_MAXSCALE_FLOW*10);
	
	/*if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		WriteConfigWord(MAXSCALE_PRESSURE_GRAPH_16,G_UPPER_MAXSCALE_NMODEPRESSURE*10);
	}
	else*/
	{
		WriteConfigWord(MAXSCALE_PRESSURE_GRAPH_16,G_UPPER_MAXSCALE_PRESSURE*10);
	}
	WriteConfigWord(MAXSCALE_VOLUME_GRAPH_16,G_UPPER_MAXSCALE_VOLUME_NEO*10);
	WriteConfigWord(MAXSCALE_FLOW_HFGRAPH_16,G_UPPER_MAXSCALE_FLOW*10);
	WriteConfigWord(MAXSCALE_VOLUME_HFGRAPH_16,G_UPPER_MAXSCALE_VOLUME_HF*10);
	WriteConfigWord(MAXSCALE_PRESSURE_HFGRAPH_16,G_UPPER_MAXSCALE_PRESSUREHF*10);
	WriteConfigWord(MINSCALE_PRESSURE_HFGRAPH_16,G_UPPER_MINSCALE_PRESSUREHF*10);
	WriteConfigWord(MAXSCALE_PRESSURE_HFPVLOOP_16,G_UPPER_MAXSCALE_PRESSUREHF*10);
	WriteConfigWord(MINSCALE_PRESSURE_HFPVLOOP_16,G_UPPER_MINSCALE_PRESSUREHF*10);
	WriteConfigWord(MAXSCALE_VOLUME_HFPVLOOP_16,G_UPPER_MAXSCALE_VOLUME_HF*10);
	WriteConfigWord(MAXSCALE_FLOW_HFVFLOOP_16,G_UPPER_MAXSCALE_FLOW*10);
	WriteConfigWord(MAXSCALE_VOLUME_HFVFLOOP_16,G_UPPER_MAXSCALE_VOLUME_HF*10);
	WriteConfigWord(MAXSCALE_PRESSURE_PVLOOP_16,G_UPPER_MAXSCALE_PRESSURE*10);
	WriteConfigWord(MAXSCALE_VOLUME_PVLOOP_16,G_UPPER_MAXSCALE_VOLUME_NEO*10);
	WriteConfigWord(MAXSCALE_VOLUME_VFLOOP_16,G_UPPER_MAXSCALE_VOLUME_NEO*10);
	WriteConfigWord(MAXSCALE_FLOW_VFLOOP_16,G_UPPER_MAXSCALE_FLOW*10);
	WriteConfigByte(IERELATIONMODE_8,RM_SET_ItoBPM);
	WriteConfigByte(LEAKCOMPENSATIONOFF_8, FALSE); 
	WriteConfigWord(SERVICE_YEAR_16, (BYTE)0); //pro, added
	WriteConfigByte(SERVICE_MONTH_8, (BYTE)0); //pro, added
	WriteConfigByte(SERVICE_DAY_8, (BYTE)0); //pro, added


	SetFRAMDefaultFactoryValues();

	return bResult;
}


// **************************************************************************
// Set one register
// **************************************************************************
bool CInterfaceI2C::SetRegister8(BYTE chDevAddr, BYTE chReg, BYTE chValue)
{
	bool bReturn=false;

	m_bWriteByte=true;
	m_chValue=chValue;
	m_chRegister1=chReg;
	m_chDevAddr=chDevAddr;
	m_bReadI2C=false;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			bReturn=true;

			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
		}
		break;
	}
	Sleep(1);
	return bReturn;
}

//***************************************************************************
// Read one/two register
//***************************************************************************
BYTE CInterfaceI2C::GetRegister8(BYTE chDevAddr, BYTE chReg)
{

	m_chRegister1=chReg;
	m_chDevAddr=chDevAddr;
	m_bReadI2C=true;
	m_bReadByte=true;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
			m_chReturn1 = 0;
		}
		break;
	}
	Sleep(1);
	return m_chReturn1;
}
WORD CInterfaceI2C::GetRegister16(BYTE chDevAddr, BYTE chReg1, BYTE chReg2)
{
	SHORT sValue=0;
	
	m_chDevAddr=chDevAddr;
	m_chRegister1=chReg1;
	m_chRegister2=chReg2;
	m_bReadByte=false;
	m_bReadI2C=true;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			sValue=MAKEWORD(m_chReturn2,m_chReturn1);

			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
			m_chReturn1 = 0;
		}
		break;
	}
	Sleep(1);
	return sValue; 
}

//***************************************************************************
// Set one register of config FRAM
//***************************************************************************
bool CInterfaceI2C::SetConfigRegister8(BYTE chDevAddr, WORD wReg, BYTE cValue)
{
	bool bReturn=false;
	
	m_bWriteByte=true;
	m_wRegister=wReg;
	m_chDevAddr=chDevAddr;
	m_chValue=cValue;
	m_bReadI2C=false;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			bReturn=true;

			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
		}
		break;
	}
	Sleep(1);
	return bReturn;
}
BYTE CInterfaceI2C::GetConfigRegister8(BYTE chDevAddr, WORD wReg)
{
	
	m_bReadI2C=true;
	m_bReadByte=true;
	m_chDevAddr=chDevAddr;
	m_wRegister=wReg;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
			m_chReturn1 = 0;
		}
		break;
	}
	Sleep(1);
	return m_chReturn1;
}
//***************************************************************************
// Set two register of config FRAM
//***************************************************************************
bool CInterfaceI2C::SetConfigRegister16(BYTE chDevAddr, WORD wReg, SHORT wValue)
{
	bool bReturn=false;
	

	m_bWriteByte=false;
	m_wValue=wValue;
	m_wRegister=wReg;
	m_chDevAddr=chDevAddr;
	m_bReadI2C=false;
	

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			bReturn=true;
			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
		}
		break;
	}
	Sleep(1);
	return bReturn;
}
SHORT CInterfaceI2C::GetConfigRegister16(BYTE chDevAddr, WORD wReg, bool bIgnoreAlarm)
{
	//NI2C_MSG_HEADER msg[2];               /* One write, one read message */
	////BYTE buffer[4]; 
	SHORT sValue=0;/* 1 byte write, 1 byte read */

	
	
	m_bReadI2C=true;
	m_bReadByte=false;
	m_chDevAddr=chDevAddr;
	m_wRegister=wReg;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			sValue=MAKEWORD(m_chReturn2,m_chReturn1);

			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);

			
		}
		break;
	}
	Sleep(1);
	return sValue;
}




// **************************************************************************
// FRAM read / write
// **************************************************************************
bool CInterfaceI2C::SetByteFRAMRegister(BYTE chDevAddr, WORD wReg, BYTE chValue)
{
	bool bReturn=false;
	
	
	m_bReadI2C=false;
	m_bWriteByte=true;
	m_chDevAddr=chDevAddr;
	m_wRegister=wReg;
	m_chValue=chValue;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			bReturn=true;
			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
		}
		break;
	}
	Sleep(0);
	return bReturn;
}
BYTE CInterfaceI2C::GetByteFRAMRegister(BYTE chDevAddr, WORD wReg)
{

	
	m_bReadI2C=true;
	m_bReadByte=true;
	m_chDevAddr=chDevAddr;
	m_wRegister=wReg;

	g_eventI2Cdata.SetEvent();
	DWORD dw = ::WaitForSingleObject(g_eventI2Cdone, I2C_WAITTIME);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			if(m_bI2CerrorPending)
			{
				m_bI2CerrorPending=false;
				ResetI2Cerror();
			}
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	default:
		{
			//error 
			m_chReturn1 = 0;
			m_bI2CerrorPending=SetI2Cerror(chDevAddr);
		}
		break;
	}
	Sleep(0);
	return m_chReturn1;



}



// **************************************************************************
// FRAM config settings IO handling
// **************************************************************************
BYTE CInterfaceI2C::ReadConfigByte(WORD wReg)
{
	
	if(!m_bFRAM)
		return 0;

	EnterCriticalSection(&m_csI2C);
	scan_msg.chDevAddr=DEVICE_ADDRESS_CONFIG;

	BYTE byRes=GetConfigRegister8(scan_msg.chDevAddr,wReg);
	LeaveCriticalSection(&m_csI2C);
	return byRes;
}
bool CInterfaceI2C::WriteConfigByte(WORD wReg, BYTE bData)
{
	if(!m_bFRAM)
		return false;

	EnterCriticalSection(&m_csI2C);
	
	scan_msg.chDevAddr=DEVICE_ADDRESS_CONFIG;

	bool bRes=SetConfigRegister8(scan_msg.chDevAddr,wReg,bData);
	LeaveCriticalSection(&m_csI2C);
	return bRes;
}

SHORT CInterfaceI2C::ReadConfigWord(WORD wReg)
{
	if(!m_bFRAM)
		return 0;

	EnterCriticalSection(&m_csI2C);
	scan_msg.chDevAddr=DEVICE_ADDRESS_CONFIG;
	SHORT sh=GetConfigRegister16(scan_msg.chDevAddr,wReg);
	LeaveCriticalSection(&m_csI2C);
	return sh;
}
bool CInterfaceI2C::WriteConfigWord(WORD wReg, SHORT wData)
{
	if(!m_bFRAM)
		return false;

	EnterCriticalSection(&m_csI2C);
	scan_msg.chDevAddr=DEVICE_ADDRESS_CONFIG;
	bool bRes=SetConfigRegister16(scan_msg.chDevAddr,wReg,wData);
	LeaveCriticalSection(&m_csI2C);
	return bRes;
}

//DWORD CInterfaceI2C::ReadAccuBytes(WORD wReg,BYTE *pBuffer,WORD bufSize)
//{
//	if(!m_bACCU)
//		return 0;
//
//	scan_msg.chDevAddr=DEVICE_ADDRESS_ACCUPIC;
//
//	return GetConfigRegisterBytes(m_hI2C,scan_msg.chDevAddr,wReg,pBuffer,bufSize);
//	//return GetConfigRegister16(m_hI2C,scan_msg.chDevAddr,wReg);
//}
SHORT CInterfaceI2C::ReadAccuWord(WORD wReg)
{
	if(!m_bACCU)
		return 0;

	EnterCriticalSection(&m_csI2C);
	scan_msg.chDevAddr=DEVICE_ADDRESS_ACCUPIC;

	SHORT sh=GetConfigRegister16(scan_msg.chDevAddr,wReg,true);
	LeaveCriticalSection(&m_csI2C);
	return sh;
}
bool CInterfaceI2C::WriteAccuWord(WORD wReg, SHORT wData)
{
	if(!m_bACCU)
		return false;

	EnterCriticalSection(&m_csI2C);
	scan_msg.chDevAddr=DEVICE_ADDRESS_ACCUPIC;
	bool res=SetConfigRegister16(scan_msg.chDevAddr,wReg,wData);
	LeaveCriticalSection(&m_csI2C);
	return res;
}

// **************************************************************************
// 
// **************************************************************************
//WORD CInterfaceI2C::ReadWordMCP()
//{
//	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;
//
//	return GetRegister16(m_hI2C,scan_msg.chDevAddr,GP0_ACCESS,GP1_ACCESS);
//}
//BYTE CInterfaceI2C::ReadByteMCP()
//{
//	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;
//
//	// Port0    /////////////////////////////////////////////////////////////
//	return GetRegister8(m_hI2C,scan_msg.chDevAddr,GP0_ACCESS);
//}
// **************************************************************************
// 
// **************************************************************************
WORD CInterfaceI2C::ReadMCP(void)
{
	BYTE b0=LOBYTE(m_wUseForUDP);
	BYTE b1=HIBYTE(m_wUseForUDP);

	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;
	
	// Port0,1   ////////////////////////////////////////////////////////////
	if(b0 && b1)
	{
		EnterCriticalSection(&m_csI2C);
		WORD wRes=GetRegister16(scan_msg.chDevAddr,GP0_ACCESS,GP1_ACCESS);
		LeaveCriticalSection(&m_csI2C);
		return wRes;
	}
	// Port1    /////////////////////////////////////////////////////////////
	if(b1)
	{
		EnterCriticalSection(&m_csI2C);
		BYTE byRes1=GetRegister8(scan_msg.chDevAddr,GP1_ACCESS);
		LeaveCriticalSection(&m_csI2C);
		return byRes1;
	}
	// Port0    /////////////////////////////////////////////////////////////
	EnterCriticalSection(&m_csI2C);
	BYTE byRes2=GetRegister8(scan_msg.chDevAddr,GP0_ACCESS);
	LeaveCriticalSection(&m_csI2C);
	return byRes2;
}
BYTE CInterfaceI2C::ReadMCP_Port0(void)
{
	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;

	// Port0    /////////////////////////////////////////////////////////////
	EnterCriticalSection(&m_csI2C);
	BYTE byRes=GetRegister8(scan_msg.chDevAddr,GP0_ACCESS);
	LeaveCriticalSection(&m_csI2C);
	return byRes;
}
BYTE CInterfaceI2C::ReadMCP_Port1(void)
{
	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;

	// Port1    /////////////////////////////////////////////////////////////
	EnterCriticalSection(&m_csI2C);
	BYTE byRes=GetRegister8(scan_msg.chDevAddr,GP1_ACCESS);
	LeaveCriticalSection(&m_csI2C);
	return byRes;
}
//bool CInterfaceI2C::WriteMCP_Port0(BYTE bData)
//{
//	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;
//
//	// Port0    /////////////////////////////////////////////////////////////
//	EnterCriticalSection(&csI2C);
//	bool bRes=SetRegister8(scan_msg.chDevAddr,GP0_ACCESS,m_byGP1_ACCESS+bData);
//	LeaveCriticalSection(&csI2C);
//	return bRes;
//}
//bool CInterfaceI2C::WriteMCP_Port1(BYTE bData)
//{
//	scan_msg.chDevAddr=DEVICE_ADDRESS_MCP;
//
//	// Port1    /////////////////////////////////////////////////////////////
//	EnterCriticalSection(&csI2C);
//	bool bRes=SetRegister8(scan_msg.chDevAddr,GP1_ACCESS,m_byGP1_ACCESS+bData);
//	LeaveCriticalSection(&csI2C);
//	return bRes;
//}


// **************************************************************************
// treiberversionen auslesen
// **************************************************************************
CStringW CInterfaceI2C::GetDriverInfo()
{
	CStringW szDrive;
	DRIVER_INFO cInfo;
	//if (!DeviceIoControl(hDevice, IOCTL_DRIVER_GETINFO, NULL, 0,
	//	&cInfo, sizeof(cInfo), NULL, NULL))
	//{
	//	cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
	//	cInfo.wVerMinor = 0;
	//}
	//printf("NSPI driver V%d.%d", cInfo.wVerMajor, cInfo.wVerMinor);

	//I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, NULL, 0,&cInfo, sizeof(cInfo), NULL, NULL); 

	EnterCriticalSection(&m_csI2C);
	if(!DeviceIoControl(m_hI2C, IOCTL_DRIVER_GETINFO, NULL, 0,&cInfo, sizeof(cInfo), NULL, NULL))
	{
		cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
		cInfo.wVerMinor = 0;
	}


	szDrive.Format(_T("V%d.%d"), cInfo.wVerMajor,cInfo.wVerMinor);
	LeaveCriticalSection(&m_csI2C);
	return szDrive;

}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::StartI2CThread( void )
{
	m_bDoI2CThread=true;
	//m_pcwtI2CThread=AfxBeginThread(CI2CThread,this);

	if(m_pcwtI2CThread!=NULL)
	{
		delete m_pcwtI2CThread;
		m_pcwtI2CThread=NULL;

		if(m_hI2CThread!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hI2CThread);
			m_hI2CThread=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtI2CThread=AfxBeginThread(CI2CThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hI2CThread=m_pcwtI2CThread->m_hThread;
	m_pcwtI2CThread->m_bAutoDelete = FALSE; 
	m_pcwtI2CThread->ResumeThread();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::StopI2CThread( void )
{
	if(m_bDoI2CThread)
	{
		m_bDoI2CThread=false;
		m_eMCPstate=MCP_NONE;
		m_bFRAM=false;
		m_bACCU=false;


		g_eventI2Cdata.SetEvent();
		
		if (WaitForSingleObject(m_pcwtI2CThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			//theApp.getLog()->WriteLine(_T("#THR:009a"));
			//theApp.getLog()->WriteLine(_T("#HFO:0110"));
			if(!TerminateThread(m_pcwtI2CThread,0))
			{
				//theApp.getLog()->WriteLine(_T("#THR:009b"));
			}
		}
	}

}

//#####################################SendThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CI2CThread( LPVOID pc )
{
	try
	{
		((CInterfaceI2C*)pc)->I2CData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CI2CThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CI2CThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceI2C*)pc)->I2CData();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceI2C::I2CData(void) 
{
	//CeSetThreadPriority(m_pcwtI2CThread->m_hThread,WATCHDOGPRIO);
	CeSetThreadPriority(m_pcwtI2CThread->m_hThread,125);

	//DEVICE_ADDRESS_MCP
	NI2C_MSG_HEADER msgMCP8read[2];               /* One write, one read message */
	BYTE bufferMCP8read[2];                       /* 1 byte write, 1 byte read */

	NI2C_MSG_HEADER msgMCP16read[2];               /* One write, one read message */
	BYTE bufferMCP16read[4];                       /* 1 byte write, 1 byte read */

	NI2C_MSG_HEADER msgMCP8write[1];               // One write message, 2 bytes
	BYTE bufferMCP8write[2];

	//DEVICE_ADDRESS_ACCUPIC
	NI2C_MSG_HEADER msgACCU16read[2];               /* One write, one read message */
	BYTE bufferACCU16read[4]; 
	//SHORT sValue=0;/* 1 byte write, 1 byte read */

	NI2C_MSG_HEADER msgACCU16write[1];               // One write message, 2 bytes
	BYTE bufferACCU16write[4];

	//DEVICE_ADDRESS_CONFIG Byte
	NI2C_MSG_HEADER msgCONFIG8read[2];               /* One write, one read message */
	BYTE bufferCONFIG8read[3];                       /* 1 byte write, 1 byte read */

	//DEVICE_ADDRESS_CONFIG Word
	NI2C_MSG_HEADER msgCONFIG16read[2];               /* One write, one read message */
	BYTE bufferCONFIG16read[4]; 
	//SHORT sValue=0;/* 1 byte write, 1 byte read */

	NI2C_MSG_HEADER msgCONFIG8write[1];               // One write message, 2 bytes
	BYTE bufferCONFIG8write[3];
	
	NI2C_MSG_HEADER msgCONFIG16write[1];               // One write message, 2 bytes
	BYTE bufferCONFIG16write[4];

	bool bI2Cerror=false;
	
	do
	{
		WaitForSingleObject(g_eventI2Cdata, INFINITE);

		if(m_bDoI2CThread && m_hI2C!=INVALID_HANDLE_VALUE)
		{
			/*iCountError++;
			if(iCountError>800)
			{
				while(1)
				{
					Sleep(0);
					iCountError++;
				}
			}*/

			switch(m_chDevAddr)
			{
			case DEVICE_ADDRESS_CONFIG:
				{						
					//m_bReadI2C=false;
					if(m_bReadI2C)
					{
						m_bReadI2C=false;

						if(m_bReadByte)
						{
							msgCONFIG8read[0].chDevAddr = m_chDevAddr;
							msgCONFIG8read[0].chFlags = 0;
							msgCONFIG8read[0].wLen = 2;
							bufferCONFIG8read[0] = HIBYTE(m_wRegister);                   /* Register number for write */
							bufferCONFIG8read[1] = LOBYTE(m_wRegister);

							msgCONFIG8read[1].chDevAddr = m_chDevAddr+1;
							msgCONFIG8read[1].chFlags = 0;
							msgCONFIG8read[1].wLen = 1;
							bufferCONFIG8read[2] = 0;

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgCONFIG8read, sizeof(msgCONFIG8read), bufferCONFIG8read, sizeof(bufferCONFIG8read), NULL);
							if (msgCONFIG8read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_GET_RESULT, (LPBYTE)msgCONFIG8read, sizeof(msgCONFIG8read),bufferCONFIG8read, sizeof(bufferCONFIG8read), NULL);
							

							if (msgCONFIG8read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));
							if(bI2Cerror)
							{
								m_chReturn1=0;
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							else
							{
								m_chReturn1=bufferCONFIG8read[2];
							}
							
							m_chReturn2 = 0;
						}
						else
						{
							m_bReadByte=true;

							msgCONFIG16read[0].chDevAddr = m_chDevAddr;
							msgCONFIG16read[0].chFlags = 0;
							msgCONFIG16read[0].wLen = 2;
							bufferCONFIG16read[0] = HIBYTE(m_wRegister);                   /* Register number for write */
							bufferCONFIG16read[1] = LOBYTE(m_wRegister);

							msgCONFIG16read[1].chDevAddr = m_chDevAddr+1;
							msgCONFIG16read[1].chFlags = 0;
							msgCONFIG16read[1].wLen = 2;
							bufferCONFIG16read[2] = 0;                        /* Dummy data for read */
							bufferCONFIG16read[3] = 0;

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgCONFIG16read, sizeof(msgCONFIG16read), bufferCONFIG16read, sizeof(bufferCONFIG16read), NULL);
							if (msgCONFIG16read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_GET_RESULT, (LPBYTE)msgCONFIG16read, sizeof(msgCONFIG16read),bufferCONFIG16read, sizeof(bufferCONFIG16read), NULL);
							
							
							if (msgCONFIG16read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));
							if(bI2Cerror)
							{
								m_chReturn1=0;                        /* Dummy data for read */
								m_chReturn2=0;
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							else
							{
								m_chReturn1=bufferCONFIG16read[2];                        /* Dummy data for read */
								m_chReturn2=bufferCONFIG16read[3];
							}
							
						}

					}
					else
					{
						if(m_bWriteByte)
						{
							msgCONFIG8write[0].chDevAddr = m_chDevAddr;
							msgCONFIG8write[0].chFlags = 0;
							msgCONFIG8write[0].wLen = 3;
							bufferCONFIG8write[0] = HIBYTE(m_wRegister);                   /* Register number for write */
							bufferCONFIG8write[1] = LOBYTE(m_wRegister);
							bufferCONFIG8write[2] = m_chValue;

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgCONFIG8write, sizeof(msgCONFIG8write), bufferCONFIG8write, sizeof(bufferCONFIG8write), NULL);
							if (msgCONFIG8write[0].chFlags > 1)
							{
								//Sleep(0);
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SKIP_RESULT, (LPBYTE)msgCONFIG8write, sizeof(msgCONFIG8write),bufferCONFIG8write, sizeof(bufferCONFIG8write), NULL);
							
							
							if (msgCONFIG8write[0].chFlags > 1)
							{
								//Sleep(0);
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));
						}
						else
						{
							m_bWriteByte=true;

							msgCONFIG16write[0].chDevAddr = m_chDevAddr;
							msgCONFIG16write[0].chFlags = 0;
							msgCONFIG16write[0].wLen = 4;
							bufferCONFIG16write[0] = HIBYTE(m_wRegister);                   /* Register number for write */
							bufferCONFIG16write[1] = LOBYTE(m_wRegister);
							bufferCONFIG16write[2] = HIBYTE(m_wValue);
							bufferCONFIG16write[3] = LOBYTE(m_wValue);

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgCONFIG16write, sizeof(msgCONFIG16write), bufferCONFIG16write, sizeof(bufferCONFIG16write), NULL);
							if (msgCONFIG16write[0].chFlags > 1)
							{
								//Sleep(0);
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SKIP_RESULT, (LPBYTE)msgCONFIG16write, sizeof(msgCONFIG16write),bufferCONFIG16write, sizeof(bufferCONFIG16write), NULL);
							
							
							if (msgCONFIG16write[0].chFlags > 1)
							{
								//Sleep(0);
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));
						}
						
					}
					
					g_eventI2Cdone.SetEvent();
				}
				break;
			case DEVICE_ADDRESS_ACCUPIC:
				{
					//m_bReadI2C=false;
					if(m_bReadI2C)
					{
						m_bReadI2C=false;

						msgACCU16read[0].chDevAddr = m_chDevAddr;
						msgACCU16read[0].chFlags = 0;
						msgACCU16read[0].wLen = 2;
						bufferACCU16read[0] = HIBYTE(m_wRegister);                   /* Register number for write */
						bufferACCU16read[1] = LOBYTE(m_wRegister);

						msgACCU16read[1].chDevAddr = m_chDevAddr+1;
						msgACCU16read[1].chFlags = 0;
						msgACCU16read[1].wLen = 2;
						bufferACCU16read[2] = 0;                        /* Dummy data for read */
						bufferACCU16read[3] = 0; 

						//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
						I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgACCU16read, sizeof(msgACCU16read), bufferACCU16read, sizeof(bufferACCU16read), NULL);
						if (msgACCU16read[0].chFlags > 1)
						{
							//Sleep(0);
							bI2Cerror=true;
						}
						I2C_IOCTL(m_hI2C, IOCTL_NI2C_GET_RESULT, (LPBYTE)msgACCU16read, sizeof(msgACCU16read),bufferACCU16read, sizeof(bufferACCU16read), NULL);
						
						
						if (msgACCU16read[0].chFlags > 1)
						{
							//Sleep(0);
							bI2Cerror=true;
						}
						//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));
						if(bI2Cerror)
						{
							m_chReturn1=0;                        /* Dummy data for read */
							m_chReturn2=0;
							m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
						}
						else
						{
							m_chReturn1=bufferACCU16read[2];                        /* Dummy data for read */
							m_chReturn2=bufferACCU16read[3];
						}
						
					
					}
					else
					{
						m_bWriteByte=true;

						msgACCU16write[0].chDevAddr = m_chDevAddr;
						msgACCU16write[0].chFlags = 0;
						msgACCU16write[0].wLen = 4;
						bufferACCU16write[0] = HIBYTE(m_wRegister);                   /* Register number for write */
						bufferACCU16write[1] = LOBYTE(m_wRegister);
						bufferACCU16write[2] = HIBYTE(m_wValue);
						bufferACCU16write[3] = LOBYTE(m_wValue);

						//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
						I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgACCU16write, sizeof(msgACCU16write), bufferACCU16write, sizeof(bufferACCU16write), NULL);
						if (msgACCU16write[0].chFlags > 1)
						{
							//Sleep(0);
							m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
						}
						I2C_IOCTL(m_hI2C, IOCTL_NI2C_SKIP_RESULT, (LPBYTE)msgACCU16write, sizeof(msgACCU16write),bufferACCU16write, sizeof(bufferACCU16write), NULL);
						
						
						if (msgACCU16write[0].chFlags > 1)
						{
							//Sleep(0);
							m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
						}
						//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));
					}
					g_eventI2Cdone.SetEvent();
				}
				break;
			case DEVICE_ADDRESS_MCP:
				{
					//m_bReadI2C=false;
					if(m_bReadI2C)
					{
						m_bReadI2C=false;

						if(m_bReadByte)
						{
							msgMCP8read[0].chDevAddr = m_chDevAddr;
							msgMCP8read[0].chFlags = 0;
							msgMCP8read[0].wLen = 1;
							bufferMCP8read[0] = m_chRegister1;                    /* Register number for write */

							msgMCP8read[1].chDevAddr = m_chDevAddr+1;
							msgMCP8read[1].chFlags = 0;
							msgMCP8read[1].wLen = 1;
							bufferMCP8read[1] = 0;

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgMCP8read, sizeof(msgMCP8read), bufferMCP8read, sizeof(bufferMCP8read), NULL);
							if (msgMCP8read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_GET_RESULT, (LPBYTE)msgMCP8read, sizeof(msgMCP8read),bufferMCP8read, sizeof(bufferMCP8read), NULL);
							
							
							if (msgMCP8read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));

							if(bI2Cerror)
							{
								m_chReturn1=0;
								m_chReturn2=0;
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							else
							{
								m_chReturn1=bufferMCP8read[1];
								m_chReturn2=0;
							}
							
						}
						else
						{
							m_bReadByte=true;

							msgMCP16read[0].chDevAddr = m_chDevAddr;
							msgMCP16read[0].chFlags = 0;
							msgMCP16read[0].wLen = 2;
							bufferMCP16read[0] = m_chRegister1;                   /* Register number for write */
							bufferMCP16read[1] = m_chRegister2;

							msgMCP16read[1].chDevAddr = m_chDevAddr+1;
							msgMCP16read[1].chFlags = 0;
							msgMCP16read[1].wLen = 2;
							bufferMCP16read[2] = 0;                        /* Dummy data for read */
							bufferMCP16read[3] = 0;

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgMCP16read, sizeof(msgMCP16read), bufferMCP16read, sizeof(bufferMCP16read), NULL);
							if (msgMCP16read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_GET_RESULT, (LPBYTE)msgMCP16read, sizeof(msgMCP16read),bufferMCP16read, sizeof(bufferMCP16read), NULL);
							
							
							if (msgMCP16read[0].chFlags > 1)
							{
								//Sleep(0);
								bI2Cerror=true;
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));

							if(bI2Cerror)
							{
								m_chReturn1=0;                        /* Dummy data for read */
								m_chReturn2=0; 
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							else
							{
								m_chReturn1=bufferMCP16read[2];                        /* Dummy data for read */
								m_chReturn2=bufferMCP16read[3]; 
							}
							
						}
					}
					else
					{
						if(m_bWriteByte)
						{
							msgMCP8write[0].chDevAddr = m_chDevAddr;
							msgMCP8write[0].chFlags = 0;
							msgMCP8write[0].wLen = 2;
							bufferMCP8write[0] = m_chRegister1;
							bufferMCP8write[1] = m_chValue;

							//DEBUGMSG(TRUE, (TEXT("start I2C_IOCTL\r\n")));
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgMCP8write, sizeof(msgMCP8write), bufferMCP8write, sizeof(bufferMCP8write), NULL);
							if (msgMCP8write[0].chFlags > 1)
							{
								//Sleep(0);
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							I2C_IOCTL(m_hI2C, IOCTL_NI2C_SKIP_RESULT, (LPBYTE)msgMCP8write, sizeof(msgMCP8write),bufferMCP8write, sizeof(bufferMCP8write), NULL);
							
							if (msgMCP8write[0].chFlags > 1)
							{
								//Sleep(0);
								m_bI2CerrorPending=SetI2Cerror(m_chDevAddr);
							}
							//DEBUGMSG(TRUE, (TEXT("end I2C_IOCTL\r\n")));

						}
						else
						{
							m_bWriteByte=true;

							//error

							//I2C_IOCTL(m_hI2C, IOCTL_NI2C_SCHEDULE, (LPBYTE)msgMCP16, sizeof(msgMCP16), bufferMCP16, sizeof(bufferMCP16), NULL);
							//I2C_IOCTL(m_hI2C, IOCTL_NI2C_SKIP_RESULT, (LPBYTE)msgMCP16, sizeof(msgMCP16),bufferMCP16, sizeof(bufferMCP16), NULL);
						}
					}
					
					g_eventI2Cdone.SetEvent();
				}
				break;
			default:
				{
					//error
					continue;
				}
				break;
			}
			
		}
	}while(m_bDoI2CThread && m_hI2C!=INVALID_HANDLE_VALUE);

	//theApp.getLog()->WriteLine(_T("#THR:009"));

	return 0;
}


bool CInterfaceI2C::ReinitI2C()
{
	
	bool bRes=TRUE;

	Deinit();
	Sleep(100);

	bRes=Init(false);

	//EnterCriticalSection(&m_csI2C);
	//if(theI2CInterface)
	//{
	//	I2C->DestroyInstance();
	//}


	//I2C=CInterfaceI2C::GetInstance();

	//if(I2C==NULL)
	//{
	//	getDATAHANDLER()->SetI2CErrorCode(ERRC_I2C_INIT);
	//	bRes=FALSE;
	//}
	//else if(I2C->Init()==false)
	//{
	//	getDATAHANDLER()->SetI2CErrorCode(ERRC_I2C_INIT);
	//	bRes=FALSE;
	//}
	//else
	//{
	//	if(I2C->ScanForDevice(DEVICE_ADDRESS_MCP))
	//	{
	//		I2C->InitMCP_ControlerPIC();
	//		//I2C->SetMCPstate(true);
	//		SetI2C_MCPavailability(TRUE);
	//	}
	//	else
	//	{
	//		//I2C->SetMCPstate(false);
	//		SetI2C_MCPavailability(FALSE);
	//		getDATAHANDLER()->SetI2CErrorCode(ERRC_I2C_MCP);
	//		bRes=FALSE;
	//	}

	//	if(I2C->ScanForDevice(DEVICE_ADDRESS_CONFIG))
	//	{
	//		I2C->SetFRAMstate(true);
	//		SetI2C_FRAMavailability(TRUE);
	//	}
	//	else
	//	{
	//		I2C->SetFRAMstate(false);
	//		SetI2C_FRAMavailability(FALSE);
	//		getDATAHANDLER()->SetI2CErrorCode(ERRC_I2C_FRAM);
	//		bRes=FALSE;
	//	}

	//	if(I2C->ScanForDevice(DEVICE_ADDRESS_ACCUPIC))
	//	{
	//		I2C->SetACCUstate(true);
	//		SetI2C_ACCUavailability(TRUE);
	//	}
	//	else
	//	{
	//		I2C->SetACCUstate(false);
	//		SetI2C_ACCUavailability(FALSE);
	//		getDATAHANDLER()->SetI2CErrorCode(ERRC_I2C_ACCU);
	//		bRes=FALSE;
	//	}


	//}

	//if(bRes)
	//	ResetI2Cerror();

	//LeaveCriticalSection(&m_csI2C);

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::SetI2C_MCPavailability(BOOL state)
{
	m_bI2C_MCPavailability=state;
}
BOOL CInterfaceI2C::IsI2C_MCPavailability()
{
	return m_bI2C_MCPavailability;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::SetI2C_FRAMavailability(BOOL state)
{
	m_bI2C_FRAMavailability=state;
}
BOOL CInterfaceI2C::IsI2C_FRAMavailability()
{
	return m_bI2C_FRAMavailability;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::SetI2C_ACCUavailability(BOOL state)
{
	m_bI2C_ACCUavailability=state;
}
BOOL CInterfaceI2C::IsI2C_ACCUavailability()
{
	return m_bI2C_ACCUavailability;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceI2C::ResetI2Cerror()
{
	m_dwLastI2CError=0;
	m_bI2CError=false;
	m_bAlarmSet=false;
}
bool CInterfaceI2C::SetI2Cerror(BYTE iAdresse,bool bStartup)
{
	bool bPending=false;
	int iBit=0;
	switch(iAdresse)
	{
	case DEVICE_ADDRESS_MCP:
		{
			iBit=ERRC_I2C_MCP;
		}
		break;
	case DEVICE_ADDRESS_CONFIG:
		{
			iBit=ERRC_I2C_FRAM;
		}
		break;
	case DEVICE_ADDRESS_ACCUPIC:
		{
			iBit=ERRC_I2C_ACCU;
		}
		break;
	default:
		{
			iBit=ERRC_I2C_INIT;
		}
		break;
	}
	
	if(!bStartup)
	{
		if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_I2C())
		{
			if(!m_bI2CError)
			{
				m_dwLastI2CError=GetTickCount();
				m_bI2CError=true;
				bPending=true;
			}
			else
			{
				if(isSafeTickCountDelayExpired(m_dwLastI2CError, I2CDELAYERROR) && m_dwLastI2CError!=0 && !m_bAlarmSet)
				{
					SetI2CErrorCode(iBit);
					if(iAdresse!=0)
					{
						m_bAlarmSet=true;
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_I2C);
					}
				}
				else
					bPending=true;
			}

		}
	}
	
	return bPending;
}

void CInterfaceI2C::SetI2CErrorCode(int bit)
{
	m_iI2CErrorCodeBits=SetBit(m_iI2CErrorCodeBits,bit);
}
void CInterfaceI2C::DeleteI2CErrorCode(int bit)
{
	m_iI2CErrorCodeBits=DeleteBit(m_iI2CErrorCodeBits,bit);
}
void CInterfaceI2C::DeleteI2CErrorCode()
{
	m_iI2CErrorCodeBits=0;
}
int CInterfaceI2C::GetI2CErrorCode()
{
	return m_iI2CErrorCodeBits;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceI2C::SetBit(int x, unsigned int n)
{
	//x = x | (1 << n);
	x = x | n;

	return x;
}
int CInterfaceI2C::DeleteBit(int x, unsigned int n)
{
	//x = x & ~(1 << n);
	x = x & ~n;

	return x;
}